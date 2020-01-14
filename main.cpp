#include <iostream>
#include <sstream>
#include <list>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "Chess.hpp"


typedef struct{
    std::string msg;
    int ms;
} Aviso;
typedef struct {
    std::list<Aviso> lista;
    void update(int ms){
        auto i = lista.begin();
        while(i != lista.end()){
            i->ms -= ms;
            if(i->ms < 0){
                i = lista.erase(i);
            }else{
                i++;
            }
        }
    }
    void avisar(const std::string& str, unsigned m=1000){
        Aviso a;
        a.msg = str; a.ms = m;
        lista.push_back(a);
    }
    std::string to_string()const{
        std::stringstream ss;
        for(auto a : lista){
            ss << a.msg << "\n";
        }
        return ss.str();
    }
} ListaAvisos;

int main(int argc, char **argv){
    const std::map<Equipo, std::string> nombreEquipo = {
        {BLANCAS, "BLANCAS"},
        {NEGRAS, "NEGRAS"}
    };
    // constants
    const unsigned n = 30;
    // window and view
    sf::RenderWindow win;
    std::cout << "Configurando ventana" << std::endl;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    win.create(sf::VideoMode(800,600), "Chess",sf::Style::Titlebar | sf::Style::Close, settings);
    win.setFramerateLimit(45);
    
    sf::View view(sf::FloatRect(-1,-1,8*n+2,8*n+2));
    sf::View viewInfo(sf::FloatRect(1000,0,220,800));
    
    
    view.setViewport(sf::FloatRect(0,0,600.0/800,1));
    viewInfo.setViewport(sf::FloatRect(600.0/800,0,200.0/800,1));
    
    
    // Pieces
    std::cout << "Cargando textura de piezas" << std::endl;
    if(!Pieza_st::texture.loadFromFile("piezas2.png")){
        std::cerr << "No se pudo cargar las texturas" << std::endl;
        exit(1);
    }
    Pieza_st::texture.setSmooth(true);
    // Board
    std::cout << "Construyendo tablero" << std::endl;
    TableroAjedrez board(n);
    // Clock
    sf::Clock clock;
    // Text
    sf::Font font;
    std::cout << "Cargando fuentes" << std::endl;
    if(!font.loadFromFile("fonts/GOTHIC.TTF")){
        exit(1);
    }
    sf::Text info_turno;
    info_turno.setString("TURNO: BLANCAS");
    info_turno.setFont(font);
    info_turno.setPosition(1010,10);
    info_turno.setCharacterSize(24);
    
    sf::Text info;
    info.setFont(font);
    info.setPosition(1010,40);
    info.setCharacterSize(24);
    
    ListaAvisos l_avisos;
    sf::Text avisos;
    avisos.setFont(font);
    avisos.setPosition(1010,750);
    avisos.setCharacterSize(16);
    
    std::cout << "Iniciando bucle" << std::endl;
    Equipo turno = BLANCAS;
    Posicion firstClick;
    while(win.isOpen()){
        // std::cout << ".";
        sf::Event e;
        while(win.pollEvent(e)){
            switch(e.type){
                case sf::Event::Closed:{
                    win.close();
                    break;
                }
                case sf::Event::KeyPressed:{
                    if(e.key.code == sf::Keyboard::Enter){
                        // 
                    }
                    break;
                }
                case sf::Event::MouseMoved:{
                    auto mousePos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
                    Posicion mp = board.coord2pos(mousePos);
                    std::string s1 = mp.to_string() + "\n";
                    std::string s2 = board.getInfo(mp);
                    info.setString(s1+s2);
                    break;
                }
                case sf::Event::MouseButtonPressed:{
                    auto mousePos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
                    if(e.mouseButton.button == sf::Mouse::Left){
                        if(!firstClick.validar()){
                            Posicion click = board.coord2pos(mousePos);
                            auto p = board.piezaEn(click);
                            auto pm = board.posiblesMovimientos(p);
                            if(p != nullptr && p->equipo == turno){
                                firstClick.r = click.r; firstClick.c = click.c;
                                board.marcar(firstClick, sf::Color::Blue);
                                for(auto m : pm){
                                    sf::Color c = 
                                        board.piezaEn(m.destino) == nullptr ?
                                            sf::Color::White : 
                                            sf::Color::Red ;
                                    board.marcar(m.destino, c);
                                }
                            }
                        }else{
                            Movimiento m(firstClick, board.coord2pos(mousePos));
                            if(board.probar(m, turno)){
                                turno = turno == BLANCAS ? NEGRAS : BLANCAS;
                                info_turno.setString("TURNO: "+nombreEquipo.at(turno));
                            }else{
                                l_avisos.avisar("Movimiento no valido");
                            }
                            if(board.jaque(turno)){
                                l_avisos.avisar(nombreEquipo.at(turno) + " EN JAQUE");
                            }
                            firstClick.r=-1;firstClick.c=-1;
                            board.borrarMarcas();
                        }
                    }
                    break;
                }
            }
        }
        
        unsigned dt = clock.restart().asMilliseconds();
        
        avisos.setString(l_avisos.to_string());
        l_avisos.update(dt);
        
        win.clear();
        win.setView(viewInfo);
        win.draw(info_turno);
        win.draw(info);
        win.draw(avisos);
        win.setView(view);
        win.draw(board);
        win.display();
    }
    
    return 0;
}
