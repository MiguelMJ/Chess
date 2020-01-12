#include <iostream>
#include <sstream>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Chess.hpp"

sf::RenderWindow win;

int main(int argc, char **argv){
    // constants
    const unsigned n = 30;
    // window and view
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    win.create(sf::VideoMode(800,600), "Chess",sf::Style::Titlebar | sf::Style::Close, settings);
    win.setFramerateLimit(45);
    
    sf::View view(sf::FloatRect(-1,-1,8*n+2,8*n+2));
    sf::View viewInfo(sf::FloatRect(1000,0,220,800));
    
    
    view.setViewport(sf::FloatRect(0,0,600.0/800,1));
    viewInfo.setViewport(sf::FloatRect(600.0/800,0,200.0/800,1));
    
    
    // Pieces
    mtb::Pieza::tex.loadFromFile("piezas2.png");
    mtb::Pieza::tex.setSmooth(true);
    // Board
    mtb::Chess board(n);
    // Clock
    sf::Clock clock;
    sf::Time wait = sf::milliseconds(500);
    // Text
    sf::Font font;
    if(!font.loadFromFile("fonts/GOTHIC.TTF")){
        exit(1);
    }
    sf::Text info;
    info.setFont(font);
    info.setPosition(1010,10);
    info.setCharacterSize(24);
    
    sf::Vector2i selectedCell(-1, -1);
    std::vector<sf::Vector2i> possibleMoves;
    
    while(win.isOpen()){
        sf::Event e;
        while(win.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:{
                    win.close();
                    break;
                }
                case sf::Event::KeyPressed:{
                    if(e.key.code == sf::Event::ENTER){
                        std::string mov;
                        getline(cin, mov);
                        int r1 = mov[0] - '0';
                        int c1 = mov[1] - '0';
                        int r2 = mov[2] - '0';
                        int c2 = mov[3] - '0';
                        auto pm = board.getPossibleMovements(r1,c2);
                        
                    }
                    break;
                }
                case sf::Event::MouseMoved:{
                    auto mousePos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
                    info.setString(board.getInfo(board.getCell(mousePos)));
                    break;
                }
                case sf::Event::MouseButtonPressed:{
                    if(e.mouseButton.button == sf::Mouse::Left){
                        auto clickPos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
                        auto clickedCell = board.getCell(clickPos);
                        std::cout << "clicked cell: " << clickedCell.x << clickedCell.y << std::endl;
                        if(selectedCell.x == -1){
                            possibleMoves = board.getPossibleMovements(clickedCell.x, clickedCell.y, false);
                            std::cout << "possible movements: " << possibleMoves.size() << std::endl;
                            if(possibleMoves.size() > 0){
                                selectedCell = clickedCell;
                                board.mark(selectedCell.x, selectedCell.y, sf::Color::White);
                                for(auto cell : possibleMoves){
                                    auto col = board.empty(cell.x, cell.y) ? sf::Color::Blue : sf::Color::Green;
                                    board.mark(cell.x, cell.y, col);
                                }
                            }
                        }else if(clickedCell.x > -1 && clickedCell.y > -1){
                            int i=0;
                            bool found = false;
                            while(i < possibleMoves.size() && !found){
                                found = possibleMoves[i] == clickedCell;
                                i++;
                            }
                            if(found){
                                board.move(selectedCell.x, selectedCell.y, clickedCell.x, clickedCell.y);
                            }
                            possibleMoves.clear();
                            selectedCell.x = -1; selectedCell.y = -1;
                            board.resetMarks();
                        }
                    }
                    break;
                }
            }
        }
        
        unsigned dt = clock.restart().asMilliseconds();
        
        win.clear();
        win.setView(viewInfo);
        win.draw(info);
        win.setView(view);
        win.draw(board);
        win.display();
    }
    
    return 0;
}
