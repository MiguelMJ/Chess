#include "Chess.hpp"

std::string Posicion_st::to_string(){
    if(validar()){
        return std::to_string(r) + std::to_string(c);
    }else{
        return "";
    }
}
void Posicion_st::from_string(const std::string& str){   
    r = str[0] - '0';
    c = str[1] - '0';
}
Posicion_st::Posicion_st(){
    r=-1;c=-1;
}
Posicion_st::Posicion_st(const std::string& str){
    from_string(str);
}
Posicion_st::Posicion_st(unsigned rr, unsigned cc){
    r = rr;
    c = cc;
}
bool Posicion_st::validar(){
    return r < 8 && c < 8;
}
Movimiento_st::Movimiento_st(const Movimiento_st& m){
    origen=m.origen;
    destino=m.destino;
}
std::string Movimiento_st::to_string(){
    return origen.to_string() + destino.to_string();
}
Movimiento_st::Movimiento_st(const std::string& str):
    origen(str.substr(0,2)), 
    destino(str.substr(2,2)) {}
Movimiento_st::Movimiento_st(Posicion o, Posicion d):
    origen(o.r, o.c),
    destino(d.r, d.c)
    {}
bool Movimiento_st::validar(){
    return origen.validar() && destino.validar();
}
sf::Texture Pieza_st::texture;
std::string Pieza_st::to_string(){
    static const std::map<TipoPieza, std::string> nombreTipo = {
        {PEON,"PEON"},
        {TORRE,"TORRE"},
        {CABALLO,"CABALLO"},
        {ALFIL,"ALFIL"},
        {REINA,"REINA"},
        {REY,"REY"}
    };
    static const std::map<Equipo, std::string> nombreEquipo = {
        {BLANCAS,"BLANCAS"},
        {NEGRAS,"NEGRAS"}
    };
    return nombreTipo.at(tipo) + " " + nombreEquipo.at(equipo);
}

Pieza_st::Pieza_st_(TipoPieza t, Equipo e, Posicion p, bool j):
    posicion(p.r, p.c){
    tipo = t;
    equipo = e;
    jugando=j;
    unsigned top, left;
    auto texSize = texture.getSize();
    top = equipo == BLANCAS ? 0 : texSize.y/2;
    switch(tipo){
        case PEON:
            left = 5;
            break;
        case TORRE:
            left = 4;
            break;
        case CABALLO:
            left = 3;
            break;
        case ALFIL:
            left = 2;
            break;
        case REINA:
            left = 1;
            break;
        case REY:
            left = 0;
            break;
    }
    int w = texSize.x/6;
    left *= w;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(left,top,texSize.x/6,texSize.y/2));
    // std::cout << "Creada ficha: " << to_string() << std::endl;
}
Evento::Evento_st(Movimiento m, Pieza p):
    movimiento(m),
    comida(p){}
bool TableroAjedrez::insertarSiValido(ListaMovimientos &l, Movimiento m){
    bool dummy=false;
    return insertarSiValido(l, m, dummy);
}
bool TableroAjedrez::insertarSiValido(ListaMovimientos &l, Movimiento m, bool& ha_comido_antes){
    bool valido = false;
    if(m.validar() && !ha_comido_antes){
        Pieza p = piezaEn(m.origen);
        Pieza d = piezaEn(m.destino);
        bool come = p != nullptr && 
                    d != nullptr && 
                    d->equipo != p->equipo;
        /*if(come){
            std::cout << p->to_string() << " come " << d->to_string() << std::endl;
        }*/
        valido = p != nullptr && (d == nullptr || come);
        ha_comido_antes = ha_comido_antes || come;
        if(valido){
            l.push_back(m);
        }
    }
    return valido;
}

void TableroAjedrez::draw(sf::RenderTarget& target, sf::RenderStates states)const{
    states.transform *= getTransform();
    for(int i=0; i < 64; i++){
        target.draw(celdas[i], states);
    }
    for(auto p : blancas){
        if(p->jugando){
            target.draw(p->sprite, states);   
        }
    }
    for(auto p : negras){
        if(p->jugando){
            target.draw(p->sprite, states);   
        }
    }
    for(auto m : marcas){
        target.draw(m, states);
    }
}
void TableroAjedrez::situarSprites(){
    std::vector<std::vector<Pieza>*> eq = {&blancas, &negras};
    for(auto pl : eq){
        for(auto p : *pl){
            poner(p->posicion, p);
            auto y = (float)lado * p->posicion.r + lado*0.1;
            auto x = (float)lado * p->posicion.c + lado*0.1;
            p->sprite.setPosition(x,y);
            auto lb = p->sprite.getLocalBounds();
            sf::Vector2f sc(lb.width, lb.height);
            float tam = (float)lado*0.8;
            sf::Vector2f fac(tam/sc.x, tam/sc.y);
            p->sprite.scale(fac);
        }
    }
}

TableroAjedrez::TableroAjedrez(int size):
    cacheMovimientos(new std::map<std::string, ListaMovimientos>){
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,0))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,1))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,2))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,3))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,4))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,5))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,6))));
    blancas.push_back(Pieza(new Pieza_st(PEON, BLANCAS, Posicion(1,7))));
    blancas.push_back(Pieza(new Pieza_st(TORRE, BLANCAS, Posicion(0,0))));
    blancas.push_back(Pieza(new Pieza_st(TORRE, BLANCAS, Posicion(0,7))));
    blancas.push_back(Pieza(new Pieza_st(CABALLO, BLANCAS, Posicion(0,1))));
    blancas.push_back(Pieza(new Pieza_st(CABALLO, BLANCAS, Posicion(0,6))));
    blancas.push_back(Pieza(new Pieza_st(ALFIL, BLANCAS, Posicion(0,2))));
    blancas.push_back(Pieza(new Pieza_st(ALFIL, BLANCAS, Posicion(0,5))));
    blancas.push_back(Pieza(new Pieza_st(REINA, BLANCAS, Posicion(0,4))));
    blancas.push_back(Pieza(new Pieza_st(REY, BLANCAS, Posicion(0,3))));
    
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,0))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,1))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,2))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,3))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,4))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,5))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,6))));
    negras.push_back(Pieza(new Pieza_st(PEON, NEGRAS, Posicion(6,7))));
    negras.push_back(Pieza(new Pieza_st(TORRE, NEGRAS, Posicion(7,0))));
    negras.push_back(Pieza(new Pieza_st(TORRE, NEGRAS, Posicion(7,7))));
    negras.push_back(Pieza(new Pieza_st(CABALLO, NEGRAS, Posicion(7,1))));
    negras.push_back(Pieza(new Pieza_st(CABALLO, NEGRAS, Posicion(7,6))));
    negras.push_back(Pieza(new Pieza_st(ALFIL, NEGRAS, Posicion(7,2))));
    negras.push_back(Pieza(new Pieza_st(ALFIL, NEGRAS, Posicion(7,5))));
    negras.push_back(Pieza(new Pieza_st(REINA, NEGRAS, Posicion(7,3))));
    negras.push_back(Pieza(new Pieza_st(REY, NEGRAS, Posicion(7,4))));
    
    lado = size;
    situarSprites();
    
    sf::Color color1(204, 102, 0);
    sf::Color color2(255, 204, 102);
    for(int r = 0; r < 8; r++){
        for(int c = 0; c < 8; c++){
            sf::RectangleShape celda(sf::Vector2f(lado,lado));
            celda.setPosition(getPosition()+sf::Vector2f(lado*r,lado*c));
            celda.setFillColor(
                (r+c) % 2 == 0 ?
                color1 : color2
            );
            celdas[r*8+c] = celda;
        }
    }
}

void TableroAjedrez::mover(Movimiento m){
    Evento e(m, piezaEn(m.destino));
    if(e.comida != nullptr){
        e.comida->jugando = false;
    }
    
    auto p = piezaEn(m.origen);
    poner(m.origen, nullptr);
    poner(m.destino, p);
    p->posicion = m.destino;
    /*
    int dx = lado * (m.destino.c - m.origen.c),
             dy = lado * (m.destino.r - m.origen.r);
    p->sprite.move(dx, dy);
    */
    cacheMovimientos->clear();
    // std::cout << "Cache limpiada" << std::endl;
    historial.push_front(e);
}
void TableroAjedrez::deshacer(){
    if(!historial.empty()){
        auto e = historial.front();
        poner(e.movimiento.origen, piezaEn(e.movimiento.destino));
        poner(e.movimiento.destino, e.comida);
        if(e.comida != nullptr){
            e.comida->jugando = true;
        }
            
        historial.pop_front();
        cacheMovimientos->clear();
    }
}
bool TableroAjedrez::soloProbar(Movimiento m, Equipo e){
    bool ret = false;
    if(m.validar()){
        mover(m);
        ret = jaque(e);
        deshacer();
    }
    return ret;
}
bool TableroAjedrez::probar(Movimiento m, Equipo e){
    bool contemplado = false;
    bool produceJaque = false;
    if(m.validar()){
        auto p = piezaEn(m.origen);
        if(p != nullptr && p->equipo == e){
            auto pm = posiblesMovimientos(piezaEn(m.origen));
            auto it = pm.begin();
            while(!contemplado && it != pm.end()){
                contemplado = it->destino.r == m.destino.r &&
                            it->destino.c == m.destino.c;
                it++;
            }
        }
    }
    if(contemplado){
        auto viejaCache = cacheMovimientos;
        cacheMovimientos.reset(new std::map<std::string, ListaMovimientos>);
        produceJaque = soloProbar(m, e);
        cacheMovimientos = viejaCache;
    }
    return contemplado && !produceJaque;
}

void TableroAjedrez::poner(Posicion p, Pieza f){
    tablero[p.r][p.c] = f;
    if(f != nullptr){
        auto pos = getPosition() + sf::Vector2f(p.c * lado + lado*0.1 , p.r * lado + lado * 0.1);
        f->sprite.setPosition(pos);
    }
}

Pieza TableroAjedrez::piezaEn(Posicion p){
    return tablero[p.r][p.c];
}

ListaMovimientos TableroAjedrez::posiblesMovimientos(Pieza p){
    ListaMovimientos ret;
    if(p != nullptr){
        Posicion pos = p->posicion;
        auto guardado = cacheMovimientos->find(pos.to_string());
        if( guardado != cacheMovimientos->end()){
            cachedAccesses = cachedAccesses+1;
            return guardado->second;
        }
        Posicion dest;
        Pieza paux;
        bool comio;
        switch(p->tipo){
            case PEON:
                unsigned dr, start;
                if(p->equipo == BLANCAS){
                    start = 1;
                    dr = 1;
                }else{
                    start = 6;
                    dr = -1;
                }
                // attack
                dest.r = pos.r + dr;
                for(dest.c = pos.c-1; dest.c <= pos.c+1; dest.c += 2){
                    if(dest.validar()){
                        paux = piezaEn(dest);
                        if(paux != nullptr && paux->equipo != p->equipo){
                            ret.push_back(Movimiento(pos, dest));
                        }
                    }
                }
                // normal move
                dest.r = pos.r+dr; dest.c = pos.c;
                if(dest.validar() && piezaEn(dest) == nullptr){
                    ret.push_back(Movimiento(pos, dest));
                    // starting move
                    dest.r += dr;
                    if( dest.validar() && pos.r == start && 
                        piezaEn(dest) == nullptr){
                        ret.push_back(Movimiento(pos, dest));
                    }
                    dest.r -= dr;
                }
                break;
            case REINA:
            case TORRE:
                // arriba
                dest.r = pos.r-1; dest.c = pos.c;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest),comio)){
                    dest.r--;
                }
                // abajo
                dest.r = pos.r+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest), comio)){
                    dest.r++;
                }
                // izquierda
                dest.r = pos.r; dest.c = pos.c-1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest), comio)){
                    dest.c--;
                }
                // derecha
                dest.c = pos.c+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos,dest), comio)){
                    dest.c++;
                }
                if (p->tipo == TORRE){
                    break;
                }
            case ALFIL:
                // arriba izquierda
                dest.r = pos.r-1; dest.c = pos.c-1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r--;
                    dest.c--;
                }
                // arriba derecha
                dest.r = pos.r-1; dest.c = pos.c+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r--;
                    dest.c++;
                }
                // abajo izquierda
                dest.r = pos.r+1; dest.c = pos.c-1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r++;
                    dest.c--;
                }
                // abajo derecha
                dest.r = pos.r+1; dest.c = pos.c+1;
                comio=false;
                while(insertarSiValido(ret, Movimiento(pos, dest), comio)){
                    dest.r++;
                    dest.c++;
                }
                break;
            case CABALLO:
                dest.r = pos.r+2; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+2; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-2; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-2; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c+2;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c-2;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c+2;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c-2;
                insertarSiValido(ret, Movimiento(pos, dest));
                break;
            case REY:
                dest.r = pos.r+1; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r+1; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c+1;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c;
                insertarSiValido(ret, Movimiento(pos, dest));
                dest.r = pos.r-1; dest.c = pos.c-1;
                insertarSiValido(ret, Movimiento(pos, dest));
                break;
        }
        (*cacheMovimientos)[pos.to_string()] = ret;
        // std::cout << "almacenado movimiento desde " << pos.to_string() << std::endl;
    }
    calculatedAccesses++;
    return ret;
}

ListaMovimientos TableroAjedrez::posiblesMovimientos(Equipo e){
    const std::vector<Pieza> &equipo = e == BLANCAS ? blancas : negras;
    ListaMovimientos ret;
    for(auto p : equipo){
        if(p->jugando){
            auto pm = posiblesMovimientos(p);
            ret.insert(ret.end(),pm.begin(), pm.end());
        }
    }
    return ret;
}

bool TableroAjedrez::jaque(Equipo e){
    bool j = false;
    const std::vector<Pieza> *equipo, *contrincante;
    // determinamos los equipos
    if(e == BLANCAS){
        equipo = &blancas;
        contrincante = &negras;
    }else{
        equipo = &negras;
        contrincante = &blancas;
    }
    // encontramos la posicion del rey
    Posicion reypos;
    for(auto p : *equipo){
        if(p->tipo == REY){
            reypos = p->posicion;
            break;
        }
    }
    // comprobamos si es atacable
    for(auto p: *contrincante){
        if(p->jugando){
            auto pm = posiblesMovimientos(p);
            for(auto mov : pm){
                j = mov.destino.r == reypos.r &&
                    mov.destino.c == reypos.c;
                if(j) break;
            }
            if(j) break;
        }
    }
    return j;
}
bool TableroAjedrez::jaquemate(Equipo e){
    bool jm=true;
    Equipo contrincante = e == BLANCAS ? NEGRAS : BLANCAS;
    auto pm = posiblesMovimientos(contrincante);
    auto mov = pm.begin();
    while(mov != pm.end() && jm){
        jm = !soloProbar(*mov, e);
        mov++;
    }
    return jm;
}

std::string TableroAjedrez::getInfo(Posicion pos){
    std::string ret;
    if(pos.validar()){
        auto p = piezaEn(pos);
        if(p != nullptr){
            ret = p->to_string();
        }else{
            ret = "-";
        }
    }
    return ret;
}
Posicion TableroAjedrez::coord2pos(sf::Vector2f coo){
    Posicion ret;
    sf::Vector2f relPos = coo - getPosition();
    ret.r = relPos.y / lado;
    ret.c = relPos.x / lado;
    if(!ret.validar()){
        ret.r = -1; ret.c = -1;
    }
    return ret;
}

void TableroAjedrez::marcar(Posicion p, sf::Color c, float thickness){
    if(p.validar()){
        float grosor = (float)lado * thickness;
        float top = p.r * lado + grosor;
        float left = p.c * lado + grosor;
        float height = lado - 2 * grosor;
        float width = lado - 2 * grosor;
        sf::RectangleShape rect(sf::Vector2f(width, height));
        rect.setPosition(getPosition() + sf::Vector2f(left, top));
        rect.setFillColor(sf::Color(0,0,0,0));
        rect.setOutlineColor(c);
        rect.setOutlineThickness(grosor);
        marcas.push_back(rect);
    }
}

void TableroAjedrez::borrarMarcas(){
    marcas.clear();
}

Movimiento Jugador::jugada(){
    Movimiento ret(sigMov->origen, sigMov->destino);
    sigMov = nullptr;
    return ret;
};

bool JugadorCliente::listo(){
    sigMov != nullptr;
}
void JugadorCliente::notificar(Movimiento m){}
void JugadorCliente::preparar(Movimiento m){
    sigMov.reset(new Movimiento(m.origen,m.destino));
}
std::random_device rd;
bool JugadorIA::listo(){
    auto pm = tablero->posiblesMovimientos(equipo);
    auto azar = pm[rd() % pm.size()];
    sigMov.reset(new Movimiento(azar.origen, azar.destino));
    return true;
}
void JugadorIA::notificar(Movimiento m){}

bool JugadorOnline::listo(){
    
}
void JugadorOnline::notificar(Movimiento m){}
