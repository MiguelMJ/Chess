#ifndef __CHESS_HPP__
#define __CHESS_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
// #include <unordered_set>
#include <random>
#include <forward_list>

typedef enum {PEON, TORRE, CABALLO, ALFIL, REY, REINA} TipoPieza;
typedef enum {BLANCAS, NEGRAS} Equipo;
typedef struct Posicion_st{
    unsigned r;
    unsigned c;
    std::string to_string();
    void from_string(const std::string& str);
    Posicion_st();
    Posicion_st(const std::string& str);
    Posicion_st(unsigned rr, unsigned cc);
    bool validar();
} Posicion;
typedef struct Movimiento_st{
    Posicion origen;
    Posicion destino;
    std::string to_string();
    Movimiento_st(const Movimiento_st& m);
    Movimiento_st(const std::string& str);
    Movimiento_st(Posicion o, Posicion d);
    bool validar();
} Movimiento;
typedef std::vector<Movimiento> ListaMovimientos;
typedef struct Pieza_st_{
    static sf::Texture texture;
    sf::Sprite sprite;
    TipoPieza tipo;
    Equipo equipo;
    Posicion posicion;
    bool jugando;
    std::string to_string();
    Pieza_st_(TipoPieza t, Equipo e, Posicion p, bool j=true);
} Pieza_st;
typedef std::shared_ptr<Pieza_st> Pieza;
typedef struct Evento_st{
    Movimiento movimiento;
    Pieza comida;
    Evento_st(Movimiento m, Pieza p);
} Evento;

class TableroAjedrez: public sf::Drawable, public sf::Transformable{
private:
    std::forward_list<Evento> historial;
    std::vector<Pieza> blancas;
    std::vector<Pieza> negras;
    Pieza tablero[8][8];
    
    std::shared_ptr<std::map<std::string, ListaMovimientos> > cacheMovimientos;
    
    // FUNCIONES AUXILIARES
    bool insertarSiValido(ListaMovimientos &l, Movimiento m, bool& ha_comido_antes);
    bool insertarSiValido(ListaMovimientos &l, Movimiento m);
    bool soloProbar(Movimiento m, Equipo e);
    
    // PARA DIBUJAR
    int lado;
    sf::RectangleShape celdas[64];
    std::vector<sf::RectangleShape> marcas;
    void draw(sf::RenderTarget& target, sf::RenderStates states)const;
    void situarSprites();
public:
    TableroAjedrez(int size=10);
    void mover(Movimiento m);
    void deshacer();
    bool probar(Movimiento m, Equipo e);
    
    void poner(Posicion p, Pieza f);
    Pieza piezaEn(Posicion p);
    
    ListaMovimientos posiblesMovimientos (Pieza p);
    ListaMovimientos posiblesMovimientos (Equipo e);
    
    bool jaque(Equipo e);
    bool jaquemate(Equipo e);
    
    std::string getInfo(Posicion p);
    Posicion coord2pos(sf::Vector2f coo);
    
    void marcar(Posicion p, sf::Color, float thickness=0.1);
    void borrarMarcas();
    // for debug
    int cachedAccesses;
    int calculatedAccesses;
    
};

class Jugador{
protected:
    std::unique_ptr<Movimiento> sigMov;
public:
    Equipo equipo;
    virtual bool listo()=0;
    virtual void notificar(Movimiento m)=0;
    Movimiento jugada();
};
class JugadorCliente : public Jugador{
public:
    virtual bool listo();
    virtual void notificar(Movimiento m);
    void preparar(Movimiento m);
};
class JugadorOnline : public Jugador{
public:
    virtual bool listo();
    virtual void notificar(Movimiento m);
};
class JugadorIA : public Jugador{
public:
    TableroAjedrez* tablero;
    virtual bool listo();
    virtual void notificar(Movimiento m);
};

class Juego{
private:
    unsigned turno;
    std::shared_ptr<Jugador> jugador[2];
    TableroAjedrez tablero;
public:
    Juego(int size=10, const std::string& textureFile="piezas.png");
    void actualizar();
};

#endif
