#ifndef __NSTREAM_H__
#define __NSTREAM_H__

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <streambuf>
using namespace std;

#ifndef nullptr
#define nullptr NULL
#endif /*nullptr*/

#ifdef _MSC_VER
#include <WinSock2.h> //SOCKET, INVALID_SOCKET, closesocket
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

typedef SOCKET SocketType;
typedef int    SocketLengthType;
const SocketType NotASocket = INVALID_SOCKET;

class WinsockInitialize {
public:
    WinsockInitialize() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
            throw runtime_error("Initialize WINSOCK failed!");
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
            throw runtime_error("Incorrect WINSOCK version.");
    }

    ~WinsockInitialize() {
        WSACleanup();
    }
};

static WinsockInitialize wsi; //make sure that WSAStartup would be called to initialize the socket environment.
                              //and WSACleanup would be called when app stop.

#else /* _MSC_VER */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef int       SocketType;
typedef socklen_t SocketLengthType;
const SocketType NotASocket = SocketType(-1);

#endif /* _MSC_VER */

#ifdef _MSC_VER
namespace {
#endif /*_MSC_VER*/
    class SocketStreambuf : public streambuf {
    public:
        SocketStreambuf() {
            sock = NotASocket;
            good = false;
        }
        explicit SocketStreambuf(const short port) : sock(NotASocket), good(false) {
            open(port);
        }
        SocketStreambuf(const string &filename, const short port) : sock(NotASocket), good(false) {
            open(filename, port);
        }
        ~SocketStreambuf() { close(); }

        //called by server
        //as a server, (struct addrinfo) hints should has ai_flags AI_PASSIVE
        //and first parameter of getaddrinfo(node, service, hints, addresses) should be nullptr
        bool open(const short port) {
            addrinfo hints, *addresses = NULL;
            memset(&hints, 0, sizeof(addrinfo));
            hints.ai_family = AF_INET;       //address family : IPv4
            hints.ai_socktype = SOCK_STREAM; //socket type : stream
            hints.ai_flags = AI_PASSIVE;     //(I am server!) can be used in function : bind

            if (0 != getaddrinfo(nullptr, ShortToString(port).c_str(), &hints, &addresses))
                return false;

            //addresses may be a linked list of addrinfo (allocated in function getaddrinfo())
            //all addrinfos in addresses should be deallocate by calling function freeaddrinfo(addresses)
            //normally, we just use the first addrinfo in addresses (and normally there is only one addrinfo)

            try {
                SocketType listenerSock = NotASocket;
                try {
                    listenerSock = ::socket(addresses->ai_family, addresses->ai_socktype, addresses->ai_protocol);
                    if (NotASocket == listenerSock)
                        throw runtime_error("Couldn't create a listener socket.");

                    if (0 != ::bind(listenerSock, addresses->ai_addr, (int)addresses->ai_addrlen))
                        throw runtime_error("Couldn't bind the socket.");

                    if (-1 == ::listen(listenerSock, 1))
                        throw runtime_error("Couldn't listen for incoming connections.");

                    SocketLengthType addrSize = sizeof(sockaddr_storage);
                    sockaddr_storage theirAddr;
                    sock = ::accept(listenerSock, (struct sockaddr *)&theirAddr, &addrSize);
                    if (NotASocket == sock)
                        throw runtime_error("Failed to accept a connection.");

                    CloseSocket(listenerSock);
                    good = true; //yeah! success!
                }
                catch (const runtime_error &) {
                    if (NotASocket != listenerSock)
                        CloseSocket(listenerSock);

                    throw;
                }
                freeaddrinfo(addresses);
            }
            catch (const runtime_error &) {
                if (nullptr != addresses)
                    freeaddrinfo(addresses);
                sock = NotASocket;
                return false;
            }
            return true;
        }

        //called by client
        bool open(const string &host, const short port) {
            try {
                addrinfo hints;
                memset(&hints, 0, sizeof(addrinfo));
                hints.ai_family = AF_UNSPEC;
                hints.ai_socktype = SOCK_STREAM;

                addrinfo *addresses = nullptr;
                const int result = getaddrinfo(host.c_str(), ShortToString(port).c_str(), &hints, &addresses);
                if (0 != result)
                    throw runtime_error("Couldn't get address info for this server.");

                try {
                    if (nullptr == addresses)
                        throw runtime_error("Got server info, but there were no nodes!");

                    sock = socket(addresses->ai_family, addresses->ai_socktype, addresses->ai_protocol);
                    if (NotASocket == sock)
                        throw runtime_error("Couldn't create local socket.");

                    if (connect(sock, addresses->ai_addr, (int)addresses->ai_addrlen) == -1)
                        throw runtime_error("Unable to connect to server.");

                    freeaddrinfo(addresses);
                    good = true; //yeah! success!
                }
                catch (const runtime_error &) {
                    if (addresses != nullptr)
                        freeaddrinfo(addresses);
                    if (NotASocket != sock)
                        CloseSocket(sock);
                    throw;
                }
            }
            catch (const runtime_error &) {
                sock = NotASocket;
                return false;
            }
            return true;
        }

        bool isGood() const { return good; }
        bool isOpen() const { return sock != NotASocket; }

        bool close() {
            if (!isOpen()) return false;

            sync();
            shutdownConnection();
            return true;
        }
    protected:
        typedef traits_type::int_type int_type;
        typedef traits_type::pos_type pos_type;
        typedef traits_type::off_type off_type;

        /*
        virtual int_type basic_streambuf::underflow()
        Ensures that at least one character is available in the input area
        by updating the pointers to the input area (if needed) and reading
        more data in from the input sequence (if applicable).
        Returns the value of that character (converted to int_type with Traits::to_int_type(c))
        on success or Traits::eof() on failure.
        */
        virtual int_type underflow() {
            if (!good)
                return traits_type::eof();

            /*
            char_type* basic_streambuf::gptr() const
            Returns the pointer to the current character (get pointer) in the get area.
            char_type* basic_streambuf::eback() const
            Returns the pointer to the beginning of the get area.
            char_type* basic_streambuf::egptr() const
            Returns the pointer one past the end of the get area.

            what's get area?
            get area is input buffer of this class,you can get char from it (sgetc())
            */
            if (gptr() == egptr()) {  //current get pos is the end of input buffer, there's no data in this buffer.
                int numRead = recv(sock, readBuffer, kBufferSize, 0);

                if (-1 == numRead || 0 == numRead) {
                    good = false;
                    return traits_type::eof();
                }

                /*
                void basic_streambuf::setg( char_type* gbeg, char_type* gcurr, char_type* gend );
                Sets the values of the pointers defining the get area.
                Specifically, after the call :
                eback() == gbeg == pointer to the new beginning of the get area
                gptr() == gcurr == pointer to the new current character (get pointer) in the get area
                egptr() == gend == pointer to the new end of the get area
                */
                setg(readBuffer, readBuffer, readBuffer + numRead);
            }
            return *gptr();
        }

        /*
        virtual int_type basic_streambuf::overflow( int_type ch = Traits::eof() )
        Ensures that there is space at the put area for at least one character
        by saving some initial subsequence of characters starting at pbase() to
        the output sequence and updating the pointers to the put area (if needed).
        If ch is not Traits::eof() (i.e. Traits::eq_int_type(ch, Traits::eof()) != true),
        it is either put to the put area or directly saved to the output sequence.
        */
        virtual int_type overflow(int_type value = traits_type::eof()) {
            if (!good)
                return traits_type::eof();

            /*
            char_type* basic_streambuf::pbase() const
            Returns the pointer to the beginning ("base") of the put area
            char_type* basic_streambuf::pptr() const
            Returns the pointer to the current character (put pointer) in the put area.
            char_type* basic_streambuf::epptr() const
            Returns the pointer one past the end of the put area

            what's put area?
            put area is output buffer of this class,you can put char into it (sputc())
            */
            const char* readPtr = pbase();
            while (readPtr != pptr()) {
                const int sendResult = send(sock, readPtr, (size_t)(pptr() - readPtr), 0);
                if (-1 == sendResult) {
                    setp(nullptr, nullptr);
                    good = false;
                    return traits_type::eof();
                }

                readPtr += sendResult;
            }
            /*
            void setp( char_type* pbeg, char_type* pend )
            Sets the values of the pointers defining the put area.
            Specifically, after the call:
            pbase() == pbeg == pointer to the new beginning of the put area
            pptr() == pbeg == pointer to the current character (put pointer) in the put area
            epptr() == pend == pointer to the new end of the put area
            */
            setp(writeBuffer, writeBuffer + kBufferSize);

            if (!traits_type::eq_int_type(traits_type::eof(), value))
                sputc(value);
            return traits_type::not_eof(value);
        }

        /*
        For output streams, this typically results in writing the contents of the put area into
        the associated sequence, i.e. flushing of the output buffer.
        For input streams, this typically empties the get area and forces a re-read from
        the associated sequence to pick up recent changes.
        The default behavior (found, for example, in std::basic_stringbuf), is to do nothing.
        */
        virtual int sync() {
            //flush the output buffer (send all data)
            return traits_type::eq_int_type(overflow(traits_type::eof()), traits_type::eof()) ? -1 : 0;
        }

        /*
        virtual int_type basic_streambuf::pbackfail( int_type c = Traits::eof() )
        This protected virtual function is called by the public functions sungetc() and sputbackc().
        if parameter c is eof(),  the purpose of pbackfail() is to back up the get area by one character,
        if the associated character sequence allows this.
        if parameter c is not eof(), the purpose of pbackfail() is to place the character c
        into the get area at the position just before basic_streambuf::gptr(), and, if possible,
        to modify the associated character sequence to reflect this change.
        */
        virtual int_type pbackfail(int_type c) {
            if (eback() == egptr())
                return traits_type::eof();

            /*
            void basic_streambuf::gbump( int count )
            Skips count characters in the get area.
            This is done by advancing the get pointer by count characters.
            No checks are done for underflow.
            */
            gbump(-1);

            if (!traits_type::eq_int_type(c, traits_type::eof()))
                *gptr() = c;

            return traits_type::not_eof(c);
        }
    private:
        void shutdownConnection() {
            if (NotASocket != sock)
                CloseSocket(sock);

            sock = NotASocket;
            good = false;
        }

        static void CloseSocket(SocketType socket) {
#ifdef _MSC_VER
            closesocket(socket);
#else
            ::close(socket);
#endif
        }

        static string ShortToString(short value) {
            stringstream converter;
            converter << value;
            return converter.str();
        }
    private:
        SocketType sock;
        bool       good;
        static const size_t kBufferSize = 4096;
        char readBuffer[kBufferSize];
        char writeBuffer[kBufferSize];
    };
#ifdef _MSC_VER
};  //namespace (anonymous)
#endif /*_MSC_VER*/



class nstream : public iostream {
public:
    nstream() : iostream(nullptr), connection(new SocketStreambuf) {
        init(connection); //Sets the associated stream buffer to connection and initializes the internal state.
        tie(this);        //Sets the current tied stream to itself.
    }
    explicit nstream(short port) : iostream(nullptr), connection(new SocketStreambuf(port)) {
        init(connection);
        tie(this);
        open(port);
    }
    nstream(const string &host, short port) : iostream(nullptr), connection(new SocketStreambuf) {
        init(connection);
        tie(this);
        open(host, port);
    }
    ~nstream() {
        rdbuf(nullptr);  //set the associated stream buffer to nullptr.
        delete connection;
    }

    void open(short port) { //wait for others connect to me
        if (!connection->open(port))
            setstate(ios_base::failbit);
    }
    void open(const string &host, short port) { //connect to someone remote
        if (!connection->open(host, port))
            setstate(ios_base::failbit);
    }

    bool is_open() const {
        return connection->isOpen();
    }

    void close() {
        if (!connection->close())
            setstate(ios_base::failbit);
    }

private:
    //class SocketStreambuf;
    SocketStreambuf *connection;
};



#endif /*__NSTREAM_H__*/