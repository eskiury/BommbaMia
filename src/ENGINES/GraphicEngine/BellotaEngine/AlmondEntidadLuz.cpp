#include "AlmondEntidadLuz.h"


    AlmondEntidadLuz::AlmondEntidadLuz(){
        shaderFiguraVertex = nullptr;
        shaderLuzYFigura = nullptr; 
        shaderFigura = nullptr;
        inicializarCuboLuz();
    }
    AlmondEntidadLuz::~AlmondEntidadLuz(){

    }
    void AlmondEntidadLuz::dibujar(const glm::mat4& transformacion, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& campos, std::vector<AlmondNode*> luces){
        //dibujara el cubo
        // Usar el programa de shaders del cubo de luz
    glUseProgram(shaderProgramCUBO);

        GLuint modelLoc = glGetUniformLocation(shaderProgramCUBO, "model");
        GLuint viewLoc = glGetUniformLocation(shaderProgramCUBO, "view");
        GLuint projLoc = glGetUniformLocation(shaderProgramCUBO, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transformacion));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Vincular el VAO del cubo de luz y dibujarlo
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

    }
    void AlmondEntidadLuz::setIntensidad (glm::vec3 inten){
        intensidad = inten;
    }
    glm::vec3 AlmondEntidadLuz::getIntensidad (){
        return intensidad;
    }
    void AlmondEntidadLuz::cargarShadersLuz(AlmondShader* SFV, AlmondShader* SLYF, AlmondShader* SF){
        shaderFiguraVertex = SFV;
        shaderLuzYFigura = SLYF; 
        shaderFigura = SF;
    }

//esta funcion es solamente para pintar un cubo que servira de referencia como si fuera la luz
    void AlmondEntidadLuz::inicializarCuboLuz() {
        float vertices[] = {
            // Cara trasera
            -0.1f, -0.1f, -0.1f,  0.1f, -0.1f, -0.1f,  0.1f,  0.1f, -0.1f,
            0.1f,  0.1f, -0.1f, -0.1f,  0.1f, -0.1f, -0.1f, -0.1f, -0.1f,

            // Cara delantera
            -0.1f, -0.1f,  0.1f,  0.1f, -0.1f,  0.1f,  0.1f,  0.1f,  0.1f,
            0.1f,  0.1f,  0.1f, -0.1f,  0.1f,  0.1f, -0.1f, -0.1f,  0.1f,

            // Cara izquierda
            -0.1f, -0.1f, -0.1f, -0.1f,  0.1f, -0.1f, -0.1f,  0.1f,  0.1f,
            -0.1f,  0.1f,  0.1f, -0.1f, -0.1f,  0.1f, -0.1f, -0.1f, -0.1f,

            // Cara derecha
            0.1f, -0.1f, -0.1f,  0.1f,  0.1f, -0.1f,  0.1f,  0.1f,  0.1f,
            0.1f,  0.1f,  0.1f,  0.1f, -0.1f,  0.1f,  0.1f, -0.1f, -0.1f,

            // Cara inferior
            -0.1f, -0.1f, -0.1f,  0.1f, -0.1f, -0.1f,  0.1f, -0.1f,  0.1f,
            0.1f, -0.1f,  0.1f, -0.1f, -0.1f,  0.1f, -0.1f, -0.1f, -0.1f,

            // Cara superior
            -0.1f,  0.1f, -0.1f,  0.1f,  0.1f, -0.1f,  0.1f,  0.1f,  0.1f,
            0.1f,  0.1f,  0.1f, -0.1f,  0.1f,  0.1f, -0.1f,  0.1f, -0.1f
        };

        //guarda los vertices
        glGenVertexArrays(1, &lightVAO);
        glGenBuffers(1, &lightVBO);
        
        glBindVertexArray(lightVAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        posicion.x = 1.2f;
        posicion.y = 1.0f;
        posicion.z = 2.0f;
    }

    void AlmondEntidadLuz::crearPrograma(){
        //compruebo que estan los shaders y creo los programas en este caso dos uno para la luz y otro para el cubo que representa la luz
        if(shaderFiguraVertex != nullptr && shaderLuzYFigura != nullptr){
            //recibir el gluint de los shaders para crear el programa
            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, shaderFiguraVertex->getShader());
            glAttachShader(shaderProgram, shaderLuzYFigura->getShader());
            glLinkProgram(shaderProgram);

            GLint success;
            GLchar infoLog[512];

            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                std::cerr << "Error linkeando el Shader Program: " << infoLog << std::endl;
            }
        }
        if(shaderFigura != nullptr && shaderFiguraVertex != nullptr){
            //recibir el gluint de los shaders para crear el programa
            shaderProgramCUBO = glCreateProgram();
            glAttachShader(shaderProgramCUBO, shaderFiguraVertex->getShader());
            glAttachShader(shaderProgramCUBO, shaderFigura->getShader());
            glLinkProgram(shaderProgramCUBO);

            GLint success;
            GLchar infoLog[512];

            glGetProgramiv(shaderProgramCUBO, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgramCUBO, 512, NULL, infoLog);
                std::cerr << "Error linkeando el Shader Program: " << infoLog << std::endl;
            }
        }
    }

    void AlmondEntidadLuz::ConfigurarLuzDireccional(glm::vec3 dir, glm::vec3 col, float ambStrength, float diffStrength, float specStrength) {
        tipoLuz = 0;
        direccion = dir;
        intensidad = col;
        ambientStrength = ambStrength;
        diffuseStrength = diffStrength;
        specularStrength = specStrength;
    
        // Valores por defecto para lo que no aplica
        posicion = glm::vec3(0.0f); // No usada en direccional
        atenCte = atenLineal = atenCuadrat = 1.0f;
        apertura = penumbra = 0.0f;
    }
    
    void AlmondEntidadLuz::ConfigurarLuzPuntual(glm::vec3 pos, glm::vec3 col, float ambStrength, float diffStrength, float specStrength,
                                                float cte, float lineal, float cuadrat) {
        tipoLuz = 1;
        posicion = pos;
        intensidad = col;
        ambientStrength = ambStrength;
        diffuseStrength = diffStrength;
        specularStrength = specStrength;
    
        atenCte = cte;
        atenLineal = lineal;
        atenCuadrat = cuadrat;
    
        // No usadas en punto
        direccion = glm::vec3(0.0f);
        apertura = penumbra = 0.0f;
    }
    
    void AlmondEntidadLuz::ConfigurarLuzSpot(glm::vec3 pos, glm::vec3 dir, glm::vec3 col, float ambStrength, float diffStrength, float specStrength,
                                             float cte, float lineal, float cuadrat, float aperturaVal, float penumbraVal) {
        tipoLuz = 2;
        posicion = pos;
        direccion = dir;
        intensidad = col;
        ambientStrength = ambStrength;
        diffuseStrength = diffStrength;
        specularStrength = specStrength;
    
        atenCte = cte;
        atenLineal = lineal;
        atenCuadrat = cuadrat;
    
        apertura = aperturaVal;
        penumbra = penumbraVal;
    }

    void AlmondEntidadLuz::setPosicion(glm::vec3 luzpos){
        posicion = luzpos;
    }
   
    glm::vec3 AlmondEntidadLuz::getPosicion(){
        return posicion;
    }

    void AlmondEntidadLuz::setDireccion(glm::vec3 dir){
        direccion = dir;
    }
   
    glm::vec3 AlmondEntidadLuz::getDireccion(){
        return direccion;
    }

    void AlmondEntidadLuz::setAmbientStrength(float ambStrength){
        ambientStrength = ambStrength;
    }
   
    float AlmondEntidadLuz::getAmbientStrength(){
        return ambientStrength;
    }

    void AlmondEntidadLuz::setDiffuseStrength(float diffStrength){
        diffuseStrength = diffStrength;
    }
   
    float AlmondEntidadLuz::getDiffuseStrength(){
        return diffuseStrength;
    }

    void AlmondEntidadLuz::setSpecularStrength(float specStrength){
        specularStrength = specStrength;
    }
   
    float AlmondEntidadLuz::getSpecularStrength(){
        return specularStrength;
    }

    // Atenuación Constante
    void AlmondEntidadLuz::setAtenCte(float value) {
        atenCte = value;
    }
    float AlmondEntidadLuz::getAtenCte() {
        return atenCte;
    }

    // Atenuación Lineal
    void AlmondEntidadLuz::setAtenLineal(float value) {
        atenLineal = value;
    }
    float AlmondEntidadLuz::getAtenLineal() {
        return atenLineal;
    }

    // Atenuación Cuadrática
    void AlmondEntidadLuz::setAtenCuadrat(float value) {
        atenCuadrat = value;
    }
    float AlmondEntidadLuz::getAtenCuadrat() {
        return atenCuadrat;
    }

    // Apertura (ángulo del spot)
    void AlmondEntidadLuz::setApertura(float angle) {
        apertura = angle;
    }
    float AlmondEntidadLuz::getApertura() {
        return apertura;
    }

    // Penumbra (ángulo externo para suavizado)
    void AlmondEntidadLuz::setPenumbra(float angle) {
        penumbra = angle;
    }
    float AlmondEntidadLuz::getPenumbra() {
        return penumbra;
    }

    // Tipo de luz
    void AlmondEntidadLuz::setTipoLuz(int tipo) {
        tipoLuz = tipo;
    }
    int AlmondEntidadLuz::getTipoLuz() {
        return tipoLuz;
    }