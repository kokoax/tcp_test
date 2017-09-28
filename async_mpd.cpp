#include <stdio.h>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <unistd.h>

int main(void) {
  // 各々のOSのI/O制御への橋渡しをするクラスらしい. 詳しくはわからん
  boost::asio::io_service io;

  boost::asio::ip::tcp::socket sock(io);

  boost::asio::ip::tcp::endpoint endpoint = boost::asio::ip::tcp::endpoint{
    boost::asio::ip::address::from_string("127.0.0.1"),   // IPv4 address
    6600                                                  // port
  };

  sock.connect(endpoint);

  boost::asio::streambuf request;
  std::ostream request_ostream(&request);
  request_ostream << "currentsong\n";

  boost::system::error_code error;

  // MPDにデータをwrite
  boost::asio::write(sock,request);

  boost::asio::streambuf buffer;

  // MPDが送りつけてくるデータの最後の行は成功なら "OK\n"なのでそこまで読み込む
  boost::asio::read_until(sock,buffer,"OK\n");
  if(error && error != boost::asio::error::eof){
    std::cout << "receive failed" << std::endl;
  } else {
    std::cout << &buffer;
  }

  return 0;
}
