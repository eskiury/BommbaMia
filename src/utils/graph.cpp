#include "graph.h"

//=============Clase Conexion=============//

Conexion::Conexion(Nodo* from, Nodo* to, double coste)
    : fromNodo(from), toNodo(to), coste(coste) {}

Nodo* Conexion::getFrom() const {
    return fromNodo;
}

Nodo* Conexion::getTo() const {
    return toNodo;
}

double Conexion::getCoste() const {
    return coste;
}

//________________________________________//

//=============Clase Nodo=============//

Nodo::Nodo(int id, int posX, int posY) : id(id), posX(posX), posY(posY) {}

Nodo::~Nodo() {
    for (auto conn : conexiones) {
        delete conn;
    }
    conexiones.clear(); // Limpia el vector después de liberar su memoria
}

int Nodo::getId() const {
    return id;
}

int Nodo::getPosX() const {
    return posX;
}

int Nodo::getPosY() const {
    return posY;
}

// void Nodo::addConexion(Nodo* toNode, double cost) {
//     conexiones.push_back(new Conexion(this, toNode, cost));
// }

const std::vector<Conexion*>& Nodo::getConexiones() const {
    return conexiones;
}

void Nodo:: addConexion(Nodo* toNodo, double coste) {

    auto it = std::find_if(conexiones.begin(), conexiones.end(),
        [toNodo](Conexion* conn) {
            return conn->getTo() == toNodo;
        });

    if (it == conexiones.end()) {
        conexiones.push_back(new Conexion(this, toNodo, coste));
    } else {
        throw std::runtime_error("La conexión ya existe entre estos nodos.");
    }
}
//____________________________________//

//=============Clase Grafo=============//

Grafo::~Grafo() {
    for (auto node : nodos) {
        delete node;
    }
}

std::vector<Conexion*> Grafo::getConexiones(Nodo* fromNodo) const {

    if( !fromNodo ) throw std::runtime_error("El nodo proporcionado es NULO.");

    return fromNodo->getConexiones();
}

Nodo* Grafo::crearNodo(int id, int posX, int posY) {
    // std::cout<<"Crear NODO\n";
    // Verificar si un nodo con el mismo ID ya existe
    auto it = std::find_if(nodos.begin(), nodos.end(),
        [id](Nodo* nodo) {
            return nodo->getId() == id;
        });

    if (it != nodos.end()) {
        throw std::runtime_error("Un nodo con este ID ya existe.");
    }

    // Crear el nodo con id, posX y posY
    Nodo* nodo = new Nodo(id, posX, posY);
    nodos.push_back(nodo);
    return nodo;
}

Nodo* Grafo::getNodoById(int id) const {
    for (auto nodo : nodos) {
        if (nodo->getId() == id) {
            return nodo;
        }
    }
    return nullptr;
}

Nodo* Grafo::getNodoByPos(int getX, int getY) const {
    for (auto nodo : nodos) {
        if ( nodo->getPosX() == getX ) {
            if ( nodo->getPosY() == getY ) {
                return nodo;
            }
        }
    }
    return nullptr;
}

void Grafo::inicializar(const std::vector<std::vector<int>>& mapaMatriz) {
    //std::cout << "Grafo sin inicializar con " << nodos.size() << " nodos.\n";
    
    // A continuación, creamos los nodos SÓLO para las celdas == 0
    // (ya que 1 será un bloque no pasable).
    std::vector<std::vector<Nodo*>> matrizNodos(mapaMatriz.size(), std::vector<Nodo*>(mapaMatriz[0].size(), nullptr));
    int contadorNodo = 0;


    int filas = matrizNodos.size();
    int cols = matrizNodos[0].size();

    for (int i = 1; i < filas-1; i++) {
        for (int j = 1; j < cols-1; j++) {
            if (mapaMatriz[i][j] == 0 || mapaMatriz[i][j] == 9) {
                // Creamos un nodo con ID = contadorNodo y posX=i, posY=j
                matrizNodos[i][j] = crearNodo(contadorNodo, i, j);
                contadorNodo++;
            }
        }
    }

    // Creamos las conexiones (arriba, abajo, izquierda, derecha)
    // sólo entre las celdas que sí tienen nodo (== 0)
    for (int i = 1; i < filas-1; i++) {
        for (int j = 1; j < cols-1; j++) {
            if (matrizNodos[i][j] != nullptr) {
                // Arriba
                if (i > 0 && matrizNodos[i-1][j] != nullptr) {
                    matrizNodos[i][j]->addConexion(matrizNodos[i-1][j], 1.0);
                }
                // Abajo
                if (i < filas - 1 && matrizNodos[i+1][j] != nullptr) {
                    matrizNodos[i][j]->addConexion(matrizNodos[i+1][j], 1.0);
                }
                // Izquierda
                if (j > 0 && matrizNodos[i][j-1] != nullptr) {
                    matrizNodos[i][j]->addConexion(matrizNodos[i][j-1], 1.0);
                }
                // Derecha
                if (j < cols - 1 && matrizNodos[i][j+1] != nullptr) {
                    matrizNodos[i][j]->addConexion(matrizNodos[i][j+1], 1.0);
                }
            }
        }
    }
    //std::cout << "[Conexiones creadas]\n";
    matrizIdNodos = matrizNodos;

    // // (Opcional) Imprimir la matriz de IDs para ver dónde hay -1 (bloque) y un ID (espacio)
    printMatrizNodos();

    // printConexionesNodos();

    //std::cout << "Grafo inicializado con " << nodos.size() << " nodos.\n";

    inicializarNidos();
    //std::cout << "Nidos iniciados.\n";
}

int Grafo::getLastNodoId() const {
    if (nodos.empty()) {
        return -1;
    }
    return nodos.back()->getId();
}

void Grafo::instalarNodo(int x, int y) {
    // Buscar el nodo en la posición especificada
    Nodo* nodo = nullptr;
    for (auto& n : nodos) {
        if (n->getPosX() == x && n->getPosY() == y) {
            nodo = n;
            break;
        }
    }

    // Si el nodo no existe, salir de la función
    if (nodo == nullptr) {
        //std::cout << "No se encontró un nodo en la posición (" << x << ", " << y << ")." << std::endl;
        return;
    }

    // Conectar el nodo con sus nodos adyacentes
    std::vector<std::pair<int, int>> adyacentes = {
        {x - 1, y}, // Izquierda
        {x + 1, y}, // Derecha
        {x, y - 1}, // Arriba
        {x, y + 1}  // Abajo
    };

    for (const auto& pos : adyacentes) {
        for (auto& n : nodos) {
            if (n->getPosX() == pos.first && n->getPosY() == pos.second) {
                nodo->addConexion(n, 1.0); // Conectar con el nodo adyacente
                n->addConexion(nodo, 1.0); // Conectar el adyacente de vuelta al nodo
                //std::cout << "Conexión bidireccional establecida entre (" << x << ", " << y 
                //          << ") y (" << pos.first << ", " << pos.second << ")." << std::endl;
            }
        }
    }

    // Actualizar la referencia en la matriz (si aplica)
    matrizIdNodos[x][y] = nodo;

    // printMatrizNodos();
}


void Grafo::printMatrizNodos() const {
    //std::cout << "Matriz de Nodos (ID):\n";
    int filas = matrizIdNodos.size();
    int cols = matrizIdNodos[0].size();
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrizIdNodos[i][j] != nullptr) {
                //std::cout << std::setw(3) << matrizIdNodos[i][j]->getId() << " ";
            } else {
                //std::cout << std::setw(3) << -1 << " ";
            }
        }
        //std::cout << std::endl;
    }
}

void Grafo::printConexionesNodos() const {
    //std::cout << "Conexiones entre Nodos:\n";
    int filas = matrizIdNodos.size();
    int cols = matrizIdNodos[0].size();

    //std::cout << "Filas: " << filas << ", Columnas: " << cols << std::endl;
    //std::cout << "Grafo en la segunda impresión: nodos.size() = " << nodos.size() << std::endl;
    for (size_t i = 0; i < nodos.size(); i++) {
        //td::cout << "nodo[" << i << "] = " << nodos[i] << std::endl;
    }

    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 15; j++) {
            if (matrizIdNodos[i][j] == nullptr) std::cout << "(N) ";
            //else std::cout << matrizIdNodos[i][j]->getId() << " ";
        }
        // std::cout << "\n";
    }

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            if (matrizIdNodos[i][j] != nullptr) {
                //std::cout << "Nodo " << matrizIdNodos[i][j]->getId() << " conexiones: ";
                const auto& conexiones = matrizIdNodos[i][j]->getConexiones();
                for (const auto& conexion : conexiones) {
                    //std::cout << conexion->getTo()->getId() << " (costo: " << conexion->getCoste() << "), ";
                }
                //std::cout << std::endl;
            }
        }
    }
}

void Grafo::limpiarNodos() {
    //std::cout << "Limpiando nodos...\n";

    // Liberar los nodos una vez, utilizando nodos como referencia principal
    for (Nodo* nodo : nodos) {
        delete nodo; // Libera cada nodo
    }
    nodos.clear(); // Limpia el vector de nodos

    // Limpiar la matriz, pero sin liberar los nodos de nuevo
    for (auto& fila : matrizIdNodos) {
        fila.clear(); // Limpia cada fila
    }
    matrizIdNodos.clear(); // Limpia el vector principal

    for (auto& nido : nidos) {
        nido.clear(); // Limpia cada nido
    }

    //std::cout << "Tamaño de nodos: " << nodos.size() << std::endl;
    //std::cout << "Tamaño de matrizIdNodos: " << matrizIdNodos.size() << std::endl;

    //std::cout << "Todos los nodos y conexiones han sido limpiados." << std::endl;
}

void Grafo::actualizarZonaInfluencia(int x, int y, const std::vector<std::vector<int>>& mapa, bool poner_quitar) {
    Nodo* nodo = getNodoByPos(x, y);
    if (nodo == nullptr) {
        // std::cerr << "Error: No se encontró un nodo en la posición (" << x << ", " << y << ").\n";
        return;
    }

    int valorTile = mapa[x][y];
    int suma_o_resta = 1;
    int radio = 0;
    float valor = 0.0f;
    float multiplicador = 0.0f;

    if(valorTile > 9 && valorTile <18) { //POWER_UP
        suma_o_resta = -1;
        radio = K_radio_PowerUp;
        valor = K_valor_PowerUp;
        multiplicador = K_multiplicador_PowerUp;
    } else if (valorTile == 7) { //BOMBA
        radio = K_radio_Bomba;
        valor = K_valor_Bomba;
        multiplicador = K_multiplicador_Bomba;
    } else if(valorTile == 8) {   //FUEGO
        radio = K_radio_Fuego;
        valor = K_valor_Fuego;
        multiplicador = K_multiplicador_Fuego;
    }else { //No es un tile especial
        return;
    }

    if(poner_quitar == false){ //Si esta puesto en quitar
        suma_o_resta * -1;
    }

    std::unordered_set<int> idsNodosVisitados;
    Nodo* nodoActual = getNodoByPos(x, y);

    float coste = 0.0f;
    for(int w=0;w<radio;w++){
        for(int i=y-w;i<=y+w;i++){
            for(int j=x-w;j<=x+w;j++){
                if(i < mapa.size() && i >= 0 && j < mapa[0].size() && j >= 0 ){
                    if(i==y-w || i==y+w || j==x-w || j==x+w){
                        nodoActual = getNodoByPos(j, i);
                        if (nodoActual == nullptr) {
                            continue;
                        }
                        idsNodosVisitados.insert(nodoActual->getId());

                        for (auto& conec : nodoActual->getConexiones()) {
                            if(conec->getTo() != nullptr){
                                //Busca si el nodo ya fue visitado
                                if (idsNodosVisitados.find(conec->getTo()->getId()) == idsNodosVisitados.end()) {
                                    // Calcua el peso de la conexión
                                    float costeTotal = conec->getCoste() + (((valor * multiplicador) / (1 + w)) * suma_o_resta);
                                    if (costeTotal < 0) {
                                        costeTotal = 0;
                                    }
                                    conec->setCoste(costeTotal);

                                    // Actualizar la conexión inversa
                                    Nodo* nodoDestino = conec->getTo();
                                    for (auto& conecInversa : nodoDestino->getConexiones()) {
                                        if (conecInversa->getTo() != nullptr && conecInversa->getTo()->getId() == nodoActual->getId()) {
                                            conecInversa->setCoste(costeTotal);
                                            break;  // Salimos una vez encontrada la conexión inversa
                                        }
                                    }
                                }                             
                            }
                        }
                    }
                }
            }
        }
    }
}

void Grafo::inicializarNidos() {
    int contador = 0;
    for (int i=0; i<nodos.size(); i++) {
        //Si nodo no se encuentra en nigun nido
        if( !nodoInNido(*nodos[i]) ){
            //Funcion recursiva comprobar nodo y conexiones de este
            
            recursivaIncluirNodosEnNidos(*nodos[i], contador);
            contador++;
        }
        
    }
}

void Grafo::recursivaIncluirNodosEnNidos(Nodo& nodo, int idNodo) {
    nidos[idNodo].push_back(&nodo);

    for(int i=0; i<nodo.getConexiones().size(); i++) {
        if( !nodoInNido(*nodo.getConexiones()[i]->getTo()) ){
            //Funcion recursiva comprobar nodo y conexiones de este
            recursivaIncluirNodosEnNidos(*nodo.getConexiones()[i]->getTo(), idNodo);
        }
    }
}

bool Grafo::nodoInNido(Nodo& nodo) const {
    for(int i=0; i<nidos.size(); i++) {
        for(int j=0; j<nidos[i].size(); j++) {
            if(nidos[i][j]->getId() == nodo.getId()) {
                return true;
            }
        }
    }
    return false;
}

std::vector<Nodo*> Grafo::buscarNidoPorNodo(Nodo* nodo) const{
    std::vector<Nodo*> nodosInNido;
    std::pair<bool, int> encontradoEnPosicion;
    for(int i=0; i<nidos.size(); i++) {
        for(int j=0; j<nidos[i].size(); j++) {
            if(nodo->getId() == nidos[i][j]->getId()) {
                encontradoEnPosicion = {true,i};
                break;
            }
        }
        if(encontradoEnPosicion.first) {
            break;
        }
    }

    if(encontradoEnPosicion.first) {
        for(int i=0; i<nidos[encontradoEnPosicion.second].size(); i++) {
            nodosInNido.push_back(nidos[encontradoEnPosicion.second][i]);
        }
    }
    return nodosInNido;
}

int Grafo::buscarIdNidoPorNodo(Nodo* nodo) const {
    int posicion = 999;
    for(int i=0; i<nidos.size(); i++) {
        for(int j=0; j<nidos[i].size(); j++) {
            if(nodo->getId() == nidos[i][j]->getId()) {
                posicion = i;
                return posicion;
            }
        }
    }
    return posicion;
}

int Grafo::buscarIdNidoPorPos(int x, int y) const {
    int posicion = 999;
    for(int i=0; i<nidos.size(); i++) {
        for(int j=0; j<nidos[i].size(); j++) {
            if(getNodoByPos(x,y)->getId() == nidos[i][j]->getId()) {
                posicion = i;
                return posicion;
            }
        }
    }
    return posicion;
}

void Grafo::integrarNodoInNido(Nodo& nodo) {
    int idAuxNido = 999;
    if(!nodoInNido(nodo)){
        for(int i=0; i<nodo.getConexiones().size(); i++) {
            //La primera de mis conexiones que tenga un nido me uno al nido
            if( nodoInNido(*nodo.getConexiones()[i]->getTo()) ){
                idAuxNido = buscarIdNidoPorNodo(nodo.getConexiones()[i]->getTo());
                nidos[idAuxNido].push_back(&nodo);
                break;
            }
        }
    }
    if(nodoInNido(nodo)) {
        for(int i=0; i<nodo.getConexiones().size(); i++) {
            if( !nodoInNido(*nodo.getConexiones()[i]->getTo()) ){
                //Si una conexion no tiene nido
                nidos[idAuxNido].push_back(nodo.getConexiones()[i]->getTo());
            }
            if( nodoInNido(*nodo.getConexiones()[i]->getTo()) ){
                //Si una conexion tiene nodo
                int idNidoAdyacente = buscarIdNidoPorNodo(nodo.getConexiones()[i]->getTo());
                if(idNidoAdyacente != 999 && idNidoAdyacente != buscarIdNidoPorNodo(&nodo)) {
                    std::vector<Nodo*> nidoAdyacente = buscarNidoPorNodo(nodo.getConexiones()[i]->getTo());
                    for(int j=0; j<nidoAdyacente.size(); j++) {
                        nidos[buscarIdNidoPorNodo(&nodo)].push_back(nidoAdyacente[j]);
                    }
                    nidos[idNidoAdyacente].clear();
                }
            }
        }
    }
}