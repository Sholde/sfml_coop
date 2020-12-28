#include <iostream>
#include <SFML/Network.hpp>

#define FAIL_CONNECTION 1

static inline void print_sfml_version()
{
  std::cout << "SFML Version " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << std::endl;
}

static inline void get_who_are_you(char &who_are_you)
{
  std::cout << "Enter (s) for server, Enter (c) for client: ";
  std::cin >> who_are_you;
}

static inline void get_password(std::string &password)
{
  std::cout << "Password: ";
  std::cin >> password;
}

int main(int argc, char **argv)
{
  print_sfml_version();

  sf::TcpSocket socket;

  char who_are_you;
  get_who_are_you(who_are_you);
  
  std::string password;
  get_password(password);
  sf::IpAddress ip {password};

  char mode;

  if (who_are_you == 's')
    {
      sf::TcpListener listener;
      listener.listen(2000);
      listener.accept(socket);

      mode = 's';
    }
  else if (who_are_you == 'c')
    {
      socket.connect(ip, 2000);
      mode = 'r';
    }
  else
    {
      return FAIL_CONNECTION;
    }

  // logger
  std::cout << std::endl;
  std::cout << "LOGGER MESSAGE: Connected" << std::endl;
  std::cout << "LOGGER MESSAGE: This is an sequential message system." << std::endl;
  std::cout << std::endl;

  std::string text;
  char buffer[200];
  std::size_t received;

  // buffer
  getline(std::cin, text);

  while (text != "exit")
    {
      if (mode == 's')
	{
	  std::cout << "Type your message: ";

	  std::getline(std::cin, text);

	  socket.send(text.c_str(), text.length() + 1);

	  mode = 'r';
	}
      else if (mode == 'r')
	{
	  std::cout << "Waiting a message..." << std::endl;
	  socket.receive(buffer, sizeof(buffer), received);

	  if (received > 0)
	    {
	      std::cout << "Received: " << buffer << std::endl;
	      mode = 's';
	    }
	}
      else
	{
	  return 2;
	}
    }

  std::cout << "Deconnected" << std::endl;

  return 0;
}
