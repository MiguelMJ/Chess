#ifndef __CHESS_HPP__
#define __CHESS_HPP__

#include "Board.hpp"
#include "util.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <map>
#include <unordered_set>

namespace mtb{
typedef enum {PEON, TORRE, CABALLO, ALFIL, REY, REINA} TipoChess;
extern const std::map<TipoChess, std::string> nameTipoChess;
class Pieza: public sf::Drawable, public sf::Transformable{
    public:
        static sf::Texture tex;
        bool blancas;
        TipoChess tipo;
        Drawable* icon;
        Pieza(TipoChess t, bool b, int size);
        void draw(sf::RenderTarget& target, sf::RenderStates states)const;
};
class Chess: public mtb::Board<8,8>{
    private:
        Pieza *game[8][8];
        std::map<std::string, std::vector<sf::Vector2i>> cachedPossibleMoves;
        void draw(sf::RenderTarget& target, sf::RenderStates states)const;
    public:
        Chess(int n=10);
        void put(Pieza *pieza, unsigned r, unsigned c);
        void reset();
        void move(const std::string);
        void move(unsigned r1, unsigned c1, unsigned r2, unsigned c2);
        std::string getInfo(sf::Vector2i pos);
        bool empty(unsigned r, unsigned c);
        void appendIfValid(unsigned r, unsigned c, bool team, std::vector<sf::Vector2i>& v, bool &choca);
        std::unordered_set<std::string> getPossibleMovements(bool equipo, bool pa=false);
        std::vector<sf::Vector2i> getPossibleMovements(const sf::Vector2f& pos, bool pa=false);
        std::vector<sf::Vector2i> getPossibleMovements(unsigned r, unsigned c, bool pa=false);
        bool check(bool team);
        bool checkmate(bool team);
};
}

#endif
