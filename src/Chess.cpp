#include "Chess.hpp"

namespace mtb{
    sf::Texture Pieza::tex;
    const std::map<TipoChess, std::string> nameTipoChess = {
        {PEON, "PEON"},
        {TORRE, "TORRE"},
        {CABALLO, "CABALLO"},
        {ALFIL, "ALFIL"},
        {REY, "REY"},
        {REINA, "REINA"}
    };
    
    Pieza::Pieza(TipoChess t, bool b, int size){
        tipo=t;
        blancas=b;
        int pos;
        switch(tipo){
            case PEON:
                pos=5;
                break;
            case TORRE:
                pos=4;
                break;
            case CABALLO:
                pos=3;
                break;
            case ALFIL:
                pos=2;
                break;
            case REY:
                pos=0;
                break;
            case REINA:
                pos=1;
                break;
        }
        auto texSize = tex.getSize();
        int top = blancas ? 0 : texSize.y / 2;
        int w = texSize.x / 6;
        auto sprite = new sf::Sprite(tex, sf::IntRect(pos*w,top,w,texSize.y/2));
        auto r = sprite->getLocalBounds();
        sprite->scale(0.8*size/r.width,0.8*size/r.height);
        icon = sprite;
    }
    void Pieza::draw(sf::RenderTarget& target, sf::RenderStates states)const{
        states.transform *= getTransform();
        target.draw(*icon, states);
    }
    void Chess::draw(sf::RenderTarget& target, sf::RenderStates states)const{
        states.transform *= getTransform();
        Board::draw(target, states);
        for(unsigned r = 0; r < 8; r++){
            for(unsigned c = 0; c < 8; c++){
                auto p = game[r][c];
                if(p != NULL) p->draw(target,states);
            }
        }
    }
    Chess::Chess(int n) : mtb::Board<8,8>(n){
        sf::Color color1(255, 206, 158);
        sf::Color color2(209, 139, 71);
        for(int r=0; r < 8; r++){
            for(int c=0; c < 8; c++){
                if((r + c) %2 == 0){
                    cellColor(r,c,color1);
                }else{
                    cellColor(r,c,color2);   
                }
                borderColor(r,c,sf::Color::Black);
                game[r][c] = NULL;
            }
        }
        reset();
    }
    void Chess::put(Pieza *pieza, unsigned r, unsigned c){
            if(pieza){
                if(game[r][c] != NULL){
                    delete game[r][c];
                }
                game[r][c] = pieza;
                pieza->setPosition(getPosition() + sf::Vector2f(size*(c + 0.1),size*(r + 0.1)));
            }
        }
    void Chess::reset(){
        for(int r = 0; r < 8; r++){
            for(int c = 0; c < 8; c++){
                if(game[r][c] != NULL) delete game[r][c];
            }
        }
        for(int i=0; i<8; i++){
            put(new Pieza(PEON, true, size),1,i);
            put(new Pieza(PEON, false, size),6,i);
        }
        put(new Pieza(TORRE, true, size), 0, 0);
        put(new Pieza(TORRE, false, size), 7, 0);
        put(new Pieza(TORRE, true, size), 0, 7);
        put(new Pieza(TORRE, false, size), 7, 7);
        put(new Pieza(CABALLO, true, size), 0, 1);
        put(new Pieza(CABALLO, false, size), 7, 1);
        put(new Pieza(CABALLO, true, size), 0, 6);
        put(new Pieza(CABALLO, false, size), 7, 6);
        put(new Pieza(ALFIL, true, size), 0, 2);
        put(new Pieza(ALFIL, false, size), 7, 2);
        put(new Pieza(ALFIL, true, size), 0, 5);
        put(new Pieza(ALFIL, false, size), 7, 5);
        put(new Pieza(REINA, true, size), 0, 3);
        put(new Pieza(REINA, false, size), 7, 4);
        put(new Pieza(REY, true, size), 0, 4);
        put(new Pieza(REY, false, size), 7, 3);
    }
    void Chess::move(const std::string& mov){
        int r1 = mov[0] - '0';
        int c1 = mov[1] - '0';
        int r2 = mov[2] - '0';
        int c2 = mov[3] - '0';
        move(r1,c1,r2,c2);
    }
    void Chess::move(unsigned r1, unsigned c1, unsigned r2, unsigned c2){
        if(r1 != r2 || c1 != c2){
            put(game[r1][c1],r2,c2);
            game[r1][c1] = NULL;
        }
        cachedPossibleMoves.clear();
    }
    std::string Chess::getInfo(sf::Vector2i pos){
        std::stringstream ret;
        if(pos.x < 8 && pos.y < 8){
            unsigned r = pos.x, c = pos.y;
            ret << "Fila:    " << r+1 << std::endl;
            ret << "Columna: " << c+1 << std::endl;
            if(game[r][c] == NULL){
                ret << "vacio";
            }else{
                ret << nameTipoChess.at((game[r][c]->tipo)) << (game[r][c]->blancas ? " BLANCAS" : " NEGRAS" ) << std::endl;
            }
        }
        return ret.str();
    }
    bool Chess::empty(unsigned r, unsigned c){
        return game[r][c] == NULL;
    }
    void Chess::appendIfValid(unsigned r, unsigned c, bool team, std::vector<sf::Vector2i>& v, bool &choca){
        if(r < 8 && c < 8){
            choca = game[r][c] != NULL;
            if(choca && game[r][c]->blancas != team || ! choca){
                v.push_back(sf::Vector2i(r,c));
            }
        }
    }
    std::unordered_set<std::string> Chess::getPossibleMovements(bool equipo, bool pa){
        std::unordered_set<std::string> ret;
        for(int r=0; r < 8; r++){
            for(int c=0; c < 8; c++){
                auto p = game[r][c];
                if(p != NULL && p->blancas == equipo){
                    auto pmov = getPossibleMovements(r,c, pa);
                    for(auto mov : pmov){
                        ret.insert(std::to_string(mov.x)+std::to_string(mov.y));
                    }
                }
            }
        }
        return ret;
    }
    std::vector<sf::Vector2i> Chess::getPossibleMovements(const sf::Vector2f& pos, bool pa){
        auto rc = getCell(pos);
        return getPossibleMovements(rc.x,rc.y, pa);
    }
    std::vector<sf::Vector2i> Chess::getPossibleMovements(unsigned r, unsigned c, bool potentialAttacks){
        std::string cellID = std::to_string(r) + std::to_string(c);
        bool excepcionPeon = game[r][c]!=NULL && game[r][c]->tipo==PEON && potentialAttacks;
        if(!excepcionPeon && cachedPossibleMoves.find(cellID) != cachedPossibleMoves.end()){
            std::cout << "Using cached movements" << std::endl;
            return cachedPossibleMoves[cellID];
        }
        std::cout << "Calculating movements" << std::endl;
        std::vector<sf::Vector2i> ret;
        if(r < 8 && c < 8 && game[r][c] != NULL){
            auto pieza = game[r][c];
            bool team = pieza->blancas;
            bool eats = false;
            switch(pieza->tipo){
                case PEON:
                {
                    int mov = team ? 1 : -1;
                    int srow = team ? 1 : 6;
                    int nrow = r + mov;
                    if(nrow < 8 && nrow > -1){
                        if(r == srow && game[nrow + mov][c] == NULL && !potentialAttacks){
                            ret.push_back(sf::Vector2i(nrow+mov,c));
                        }
                        if(game[nrow][c] == NULL && !potentialAttacks){
                            ret.push_back(sf::Vector2i(nrow,c));
                        }
                        if(c < 7 && (game[nrow][c+1] != NULL && game[nrow][c+1]->blancas != team || potentialAttacks)){
                            ret.push_back(sf::Vector2i(nrow,c+1));
                        }
                        if(c > 0 && (game[nrow][c-1] != NULL && game[nrow][c-1]->blancas != team || potentialAttacks)){
                            ret.push_back(sf::Vector2i(nrow,c-1));
                        }
                    }
                    break;
                }
                case REINA:
                case TORRE:
                {
                    for(int dr=r+1; dr < 8 && !eats; dr++){ // hacia abajo
                        appendIfValid(dr, c, team, ret, eats);
                    }
                    eats = false;
                    for(int dr=r-1; dr > -1 && !eats; dr--){ // hacia arriba
                        appendIfValid(dr, c, team, ret, eats);
                    }
                    eats = false;
                    for(int dc=c+1; dc < 8 && !eats; dc++){ // hacia derecha
                        appendIfValid(r, dc, team, ret, eats);
                    }
                    eats = false;
                    for(int dc=c-1; dc > -1 && !eats; dc--){ // hacia izquierda
                        appendIfValid(r, dc, team, ret, eats);
                    }
                }
                if(pieza->tipo == TORRE) break;
                case ALFIL:
                {
                    eats = false;
                    for(int dr=r+1, dc=c+1; dr<8 && dc<8 && !eats; dr++, dc++){ // hacia abajo derecha
                        appendIfValid(dr, dc, team, ret, eats);
                    }
                    eats = false;
                    for(int dr=r-1, dc=c+1; dr>-1 && dc<8 && !eats; dr--, dc++){ // hacia arriba derecha
                        appendIfValid(dr, dc, team, ret, eats);
                    }
                    eats = false;
                    for(int dr=r+1, dc=c-1; dr<8 && dc>-1 && !eats; dr++, dc--){ // hacia abajo izquierda
                        appendIfValid(dr, dc, team, ret, eats);
                    }
                    eats = false;
                    for(int dr=r-1, dc=c-1; dr>-1 && dc>-1 && !eats; dr--, dc--){ // hacia arriba izquierda
                        appendIfValid(dr, dc, team, ret, eats);
                    }
                    break;
                }
                case CABALLO:
                {
                    appendIfValid(r+2, c+1, team, ret, eats);
                    appendIfValid(r+2, c-1, team, ret, eats);
                    appendIfValid(r-2, c+1, team, ret, eats);
                    appendIfValid(r-2, c-1, team, ret, eats);
                    appendIfValid(r+1, c+2, team, ret, eats);
                    appendIfValid(r+1, c-2, team, ret, eats);
                    appendIfValid(r-1, c+2, team, ret, eats);
                    appendIfValid(r-1, c-2, team, ret, eats);
                    break;
                }
                case REY:
                    appendIfValid(r-1, c-1, team, ret, eats);
                    appendIfValid(r-1, c, team, ret, eats);
                    appendIfValid(r-1, c+1, team, ret, eats);
                    appendIfValid(r, c-1, team, ret, eats);
                    appendIfValid(r, c+1, team, ret, eats);
                    appendIfValid(r+1, c-1, team, ret, eats);
                    appendIfValid(r+1, c, team, ret, eats);
                    appendIfValid(r+1, c+1, team, ret, eats);
                    break;
            }
        }
        if(!excepcionPeon)
            cachedPossibleMoves[cellID] = ret;
        return ret;
    }
    bool Chess::check(bool team){
        auto vulnerable = getPossibleMovements(!team,true);
        std::string king;
        for(int r = 0; r < 8; r++){
            for(int c = 0; c < 8; c++){
                auto p = game[r][c];
                if(p!=NULL && p->tipo==REY && p->blancas==team){
                    king = std::to_string(r) + std::to_string(c);
                }
            }
        }
        return vulnerable.find(king) != vulnerable.end();
    }
    bool Chess::checkmate(bool team){
        // para cada posible movimiento de team
        auto pm = getPossibleMovements(team);
        for(){
            // probarlo
            Pieza* temp = 
            // si la casilla del rey no es atacable
                // return GOODTOGO
            // deshacerlo
            // 
        }
    }
}
