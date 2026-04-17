#include "AlmondParticula.h"

AlmondParticula::AlmondParticula(){
    shaderVertex = nullptr;
    shaderFragment = nullptr;
    VAO = 0;
    VBO = 0;
    EBO = 0;
    VBOinstancia = 0;
    setPosicion(glm::vec3(0.0f, 0.0f, 0.0f));
    setColor(glm::vec3(0.0f, 0.0f, 0.0f));
    setTamanyo(0.0f);
    setVida(0.0f);
    setVelocidad(glm::vec3(0.0f, 0.0f, 0.0f));
    glm::vec3 verti[4] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3(0.0f,  0.0f, 0.0f)
    };
    setVertices(verti);
    configurar();
    vidaInicial = vida;
    setAngulo(45.0f);
    textura = nullptr;
    setTipoDeParticula(0);
}

AlmondParticula::AlmondParticula(glm::vec3 posi,glm::vec3 col,float tam, float vida, glm::vec3 velo, glm::vec3 verti[4], AlmondShader* shaderVertexPAS,AlmondShader* shaderFragmentPAS,float ang,AlmondTextura* tex, int tip){
    shaderVertex = nullptr;
    shaderFragment = nullptr;
    VAO = 0;
    VBO = 0;
    EBO = 0;
    VBOinstancia = 0;
    shaderVertex = shaderVertexPAS;
    shaderFragment = shaderFragmentPAS;
    setPosicion(posi);
    setColor(col);
    setTamanyo(tam);
    setVida(vida);
    setVelocidad(velo);
    setVertices(verti);
    configurar();
    vidaInicial = vida;
    setAngulo(ang);
    textura = tex;
    setTipoDeParticula(tip);
}

AlmondParticula::~AlmondParticula(){

}

//---------------------------GETTERS----------------------
glm::vec3 AlmondParticula::getPosicion() const{
    return posicion;
}
glm::vec3 AlmondParticula::getColor() const{
    return color;
}
float AlmondParticula::getTamanyo() const{
    return tamanyo;
}
float AlmondParticula::getVida() const{
    return vida;
}
glm::vec3 AlmondParticula::getVelocidad() const{
    return velocidad;
}
const glm::vec3* AlmondParticula::getVertices() const { 
    return vertices; 
}
int AlmondParticula::getTipoDeParticula(){
    return tipoDeParticula;
}
//---------------------------SETTERS----------------------
void AlmondParticula::setPosicion(glm::vec3 posi){
    posicion = posi;
}
void AlmondParticula::setColor(glm::vec3 col){
    color = col;
}
void AlmondParticula::setTamanyo(float tam){
tamanyo = tam;
}
void AlmondParticula::setVida(float vid){
    vida = vid;
    vidaInicial = vid;
}
void AlmondParticula::setVelocidad(glm::vec3 vel){
    velocidad = vel;
}
void AlmondParticula::setVertices(const glm::vec3 verts[4])
{
    for (int i = 0; i < 4; ++i)
    {
        vertices[i] = verts[i];
    }
}
void AlmondParticula::setTipoDeParticula(int tip){
    tipoDeParticula = tip;
}
 //----------------------FUNCIONES PROPIAS----------------
 void AlmondParticula::configurar() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBOinstancia); // Datos dinámicos de instancia
    glGenBuffers(1, &EBO); // NUEVO: crear EBO

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Reservar espacio para quad + datos de instancia
    // std::size_t quadSize = sizeof(vertices);
    // std::size_t datosInstanciaSize = sizeof(posicion) + sizeof(color) + sizeof(tamanyo);
    // glBufferData(GL_ARRAY_BUFFER, quadSize + datosInstanciaSize, nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    // EBO: índices
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // VBO: datos dinámicos de instancia
    glBindBuffer(GL_ARRAY_BUFFER, VBOinstancia);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(float) + sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    // Atributo 1: posición
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);

    // Atributo 2: color (con alpha)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(float), (void*)sizeof(glm::vec3));
    glVertexAttribDivisor(2, 1);

    // Atributo 3: tamaño
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(float), (void*)(sizeof(glm::vec3) + sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);

    // Atributo 4: ángulo de rotación
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(float) + sizeof(float),(void*)(sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(float)));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    // Crear programa de shaders
    if(shaderVertex != nullptr && shaderFragment != nullptr){
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, shaderVertex->getShader());
        glAttachShader(shaderProgram, shaderFragment->getShader());
        glLinkProgram(shaderProgram);

        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cerr << "Error linkeando el Shader Program: " << infoLog << std::endl;
        }
    }
}

void AlmondParticula::dibujar(const glm::mat4& view, const glm::mat4& projection,const glm::vec3& campos){
    if(vida > 0.0f){
    // Calcula el alpha en función de la vida restante
    float alpha = glm::clamp(vida / vidaInicial, 0.0f, 1.0f);
    glm::vec4 colorConAlpha = glm::vec4(color, alpha);
    glUseProgram(shaderProgram);

    // Uniforms
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(campos));

   // Actualizar solo datos dinámicos de instancia
    glBindBuffer(GL_ARRAY_BUFFER, VBOinstancia);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(posicion));                        // posición
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3), sizeof(glm::vec4), glm::value_ptr(colorConAlpha));  // color + alpha
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(glm::vec3) + sizeof(glm::vec4), sizeof(float), &tamanyo);       // tamaño
    glBufferSubData(GL_ARRAY_BUFFER,sizeof(glm::vec3) + sizeof(glm::vec4) + sizeof(float),sizeof(float),&angulo);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //textura
    bool tieneTextura = (textura != nullptr);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarTextura"), tieneTextura);

    if (textura != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textura->getId());
        glUniform1i(glGetUniformLocation(shaderProgram, "particleTexture"), 0);
    }

    // Dibujar
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 1);
    glBindVertexArray(0);
    }
    
}

void AlmondParticula::actualizarParticulas(float dt){
    if(tipoDeParticula == 1){
        //actualizo el humo
        actualizarHumo(dt);
    }else if(tipoDeParticula == 2){
        //actualizo las explosiones
        actualizarExplosion(dt);
    }else if(tipoDeParticula == 3){
        actualizarChispa(dt);
    }
}

void AlmondParticula::actualizarHumo(float dt) {
    if (vida <= 0.0f) return;

    // Movimiento básico
    posicion += velocidad * dt;

    // Vida: reducir con el tiempo
    vida -= dt;

    // Expansión visual
    tamanyo += 0.1f * dt;

    // Impulso vertical más fuerte al principio (ease-out)
    float t = glm::clamp((vidaInicial - vida) / vidaInicial, 0.0f, 1.0f);
    float impulso = (1.0f - pow(t, 2.0f));  // EaseOut cuadrático

    velocidad += glm::vec3(0.0f, 1.0f * impulso, 0.0f) * dt;
    
}

void AlmondParticula::actualizarExplosion(float dt) {
    if (vida <= 0.0f) return;
    // Movimiento
    posicion += velocidad * dt;

    // Vida
    vida -= dt;

    // Desvanecimiento visual (achicar)
    tamanyo -= 0.5f * dt;
    if (tamanyo < 0.0f) tamanyo = 0.0f;

    // Frenado (resistencia al aire)
    velocidad *= 0.9f;

    // Opcional: desaturar el color con el tiempo
    float factor = vida / vidaInicial;
    color *= glm::vec3(factor);
}

void AlmondParticula::actualizarChispa(float dt) {
    if (vida <= 0.0f) return;

    posicion += velocidad * dt;
    vida -= dt;

    // Leve desvanecimiento
    // float factor = vida / vidaInicial;
    // color *= glm::vec3(factor);

    // Fricción ligera
    velocidad *= 0.95f;
}


void AlmondParticula::setAngulo(float a) {
    angulo = a;
}

float AlmondParticula::getAngulo() const {
    return angulo;
}

void AlmondParticula::setTextura(AlmondTextura* tex) {
     textura = tex; 
    }
AlmondTextura* AlmondParticula::getTextura() const { 
    return textura; 
}