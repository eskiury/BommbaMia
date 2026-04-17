#include "AlmondMalla.h"
#include "AlmondGestorRecursos.h"


    AlmondMalla::AlmondMalla(){
        
    }
    AlmondMalla::~AlmondMalla(){
        //libera la memoria de los buffers de opengl
        // glDeleteVertexArrays(1, &VAO);
        // glDeleteBuffers(1, &VBO);
        // glDeleteBuffers(1, &EBO);
    }

    AlmondMalla::AlmondMalla(const std::string& nombre){
        this->nombre = nombre;  // Asignar el nombre al atributo protegido
        gestorGuardado = nullptr; // de momento pongo el gestor a nullptr
        minVertex.x = 999999.0f;//por defecto altos, para poder usarlos de referencia al comparar luego el primero
        maxVertex.x = -999999.0f;
        minVertex.y = 999999.0f;
        maxVertex.y = -999999.0f;
        minVertex.z = 999999.0f;
        maxVertex.z = -999999.0f;
        cargarFichero(nombre);
        // std::cerr << "Creando AlmondMalla con " << vertices.size() << " vértices y " << indices.size() << " índices." << std::endl;
    }

    AlmondMalla::AlmondMalla(const std::string& nombre,AlmondGestorRecursos* gestor){
        this->nombre = nombre;  // Asignar el nombre al atributo protegido
        gestorGuardado = gestor; // de momento pongo el gestor a nullptr
        minVertex.x = 999999.0f;//por defecto altos, para poder usarlos de referencia al comparar luego el primero
        maxVertex.x = -999999.0f;
        minVertex.y = 999999.0f;
        maxVertex.y = -999999.0f;
        minVertex.z = 999999.0f;
        maxVertex.z = -999999.0f;
        cargarAlmondVertexShader();
        cargarAlmondFragmentShader();
        cargarFichero(nombre);
        // std::cerr << "Creando AlmondMalla con " << vertices.size() << " vértices y " << indices.size() << " índices." << std::endl;
    }


   void AlmondMalla::dibujar(const glm::mat4& transformacion, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& campos,std::vector<AlmondNode*> luces) {
    // Obtener el número de submallas
    // size_t numSubmallas = subMallas.size();
    // std::cout << "Número de submallas: " << numSubmallas << std::endl;
        for (const auto& subMalla : subMallas) {
            // Aquí podrías seleccionar el material según `subMalla.materialIndex`
            // y asignarle su textura antes de renderizar.

            subMalla.draw(transformacion, view, projection, materiales, materialesIndex, campos, luces); //Pasar las texturas a la submalla
        }
    }



void AlmondMalla::cargarFichero(const std::string& ruta) {
   // std::cout << "Cargando modelo 3D desde el archivo: " << nombre << std::endl;

    Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(ruta, aiProcess_Triangulate | aiProcess_FlipUVs);
        //sino se puede salta mensaje y returneamos
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Error al cargar el modelo: " << importer.GetErrorString() << std::endl;
            return;
        }
        //procesamos el nodo
        procesarNodo(scene->mRootNode, scene);
}

void AlmondMalla::procesarNodo(aiNode* nodo, const aiScene* scene){
        for (unsigned int i = 0; i < nodo->mNumMeshes; i++) {
            if (nodo->mMeshes[i] >= scene->mNumMeshes) {
                std::cerr << "Referencia de malla fuera de rango: " << nodo->mMeshes[i] << std::endl;
                continue;
            }
            aiMesh* mesh = scene->mMeshes[nodo->mMeshes[i]];
            procesarMalla(mesh, scene);
            
        }

        for (unsigned int i = 0; i < nodo->mNumChildren; i++) {
            procesarNodo(nodo->mChildren[i], scene);
        }

        // Calcula centro y tamaño del cubo
        centroBoundingBox = (minVertex + maxVertex) * 0.5f; // se multiplica por 0.5f porque sino nos da la diagonal total, no el centro.

        glm::vec3 dimensiones = maxVertex - minVertex;
        //coge el tamaño de la cara mas grande, para que no quede nada fuera del bounding box
        tamanyoBoundingBox = std::max(std::max(dimensiones.x, dimensiones.y), dimensiones.z);
}

void AlmondMalla::procesarMalla(aiMesh* mesh, const aiScene* scene) {
    SubMalla nuevaMalla;
    nuevaMalla.materialIndex = mesh->mMaterialIndex;  // Asociar la malla a su material
    // Imprimir el índice del material
    //std::cout << "Material Index de la malla: " << nuevaMalla.materialIndex << std::endl;


    if (mesh->mMaterialIndex >= scene->mNumMaterials) {
        std::cerr << "Error: Material index fuera de rango" << std::endl;
        return;
    }

    aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];

    aiString aiMatName;
    if (AI_SUCCESS != aiMat->Get(AI_MATKEY_NAME, aiMatName)) {
        std::cerr << "Advertencia: No se pudo obtener el nombre del material." << std::endl;
    }
    std::string materialNombre = aiMatName.C_Str();  // Convertir a std::string

    std::string nombre = materialNombre;
    std::string textu = "NOHAY";  // Valor predeterminado para la textura
    glm::vec3 ambient(1.0f), diffuse(1.0f), specular(0.5f);
    float shininess = 32.0f;  // Valor predeterminado

    aiColor3D color;
    float shininessVal = 0.0f;

    // Obtener propiedades del material
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
        ambient = glm::vec3(color.r, color.g, color.b);
    }

    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
        diffuse = glm::vec3(color.r, color.g, color.b);
    }

    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
        specular = glm::vec3(color.r, color.g, color.b);
    }

    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHININESS, shininessVal) && shininessVal > 0.0f) {
        shininess = shininessVal;
    }

    // Obtener ruta de la textura (si existe)
    aiString texPath;
    if (AI_SUCCESS == aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath)) {
        textu = texPath.C_Str();
    }

    // Buscar el material en el gestor de recursos
    AlmondMaterial* material = gestorGuardado->getMaterial(nombre, ambient, diffuse, specular, shininess, textu);

    if (material) {
        // Evitar agregar materiales duplicados
        if (std::find(materiales.begin(), materiales.end(), material) == materiales.end()) {
            materiales.push_back(material);
            materialesIndex.push_back(nuevaMalla.materialIndex);
        }
    }


        

    // Cargar vértices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        AlmondVertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        // Actualiza min y max globales, y voy guardando los que sean mayores o menores
        minVertex.x = std::min(minVertex.x, vertex.Position.x);
        minVertex.y = std::min(minVertex.y, vertex.Position.y);
        minVertex.z = std::min(minVertex.z, vertex.Position.z);

        maxVertex.x = std::max(maxVertex.x, vertex.Position.x);
        maxVertex.y = std::max(maxVertex.y, vertex.Position.y);
        maxVertex.z = std::max(maxVertex.z, vertex.Position.z);
        
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        } else {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        if (mesh->mTextureCoords[0]) {
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        nuevaMalla.vertices.push_back(vertex);
    }

    // Cargar índices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            nuevaMalla.indices.push_back(face.mIndices[j]);
        }
    }
    nuevaMalla.cargarShaders(shaderVertex, shaderFragment); //pado los vertex y fragment shaders a la submallas
    nuevaMalla.setupMesh();  // Configurar buffers de OpenGL

    subMallas.push_back(nuevaMalla);  // Guardar la sub-malla en la lista
}


 void AlmondMalla::cargarTexturaALaMalla(AlmondTextura* tex){
    texturas.push_back(tex);
 }


void AlmondMalla::cargarGestor(AlmondGestorRecursos* gestor){
    gestorGuardado = gestor;
}

void AlmondMalla::cargarAlmondVertexShader(){
    const std::string& ruta = "assets/shaders/shaderPrue.AlmondVertex";
    shaderVertex = gestorGuardado->getRecurso<AlmondShader>(ruta);
    
}
void AlmondMalla::cargarAlmondFragmentShader(){
    const std::string& ruta2 = "assets/shaders/shaderBasico.AlmondFragment";
    shaderFragment = gestorGuardado->getRecurso<AlmondShader>(ruta2);
}

void AlmondMalla::PasarShadersASubmallas(){
    for (auto& subMalla : subMallas) {
            subMalla.cargarShaders(shaderVertex, shaderFragment);
        }
}

// Getter para el centro del bounding box
glm::vec3 AlmondMalla::getCentroBoundingBox() const {
    return centroBoundingBox;
}

// Getter para el tamaño del bounding box (lado del cubo)
float AlmondMalla::getTamanyoBoundingBox() const {
    return tamanyoBoundingBox;
}

// Getter para el vértice mínimo
glm::vec3 AlmondMalla::getMinVertex() const {
    return minVertex;
}

// Getter para el vértice máximo
glm::vec3 AlmondMalla::getMaxVertex() const {
    return maxVertex;
}
