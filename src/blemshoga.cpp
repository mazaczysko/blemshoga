#include <SFML/Graphics.hpp>
#include <iostream>

int main( )
{
	std::cerr << "blemshoga is up!" << std::endl;
	
	sf::Window window( sf::VideoMode( 800, 600 ), "bingo bango bongo blemshoga" );

	while ( window.isOpen( ) )
	{
		sf::Event ev;
		while ( window.pollEvent( ev ) )
		{
			switch ( ev.type )
			{
				case sf::Event::Closed:
					window.close( );
					break;

				default:
					break;
			}
		}
	}

	return 0;
}
