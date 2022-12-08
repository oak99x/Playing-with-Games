#include "Textura.h"

using namespace std;

Textura::Textura(){
    id = 0;
    largura = 0;
    altura = 0;
    canais = 0;
}

//Carrega Textura
void Textura::load(string filePath)
{
	unsigned char* imgData;

	stbi_set_flip_vertically_on_load(true);
	imgData = stbi_load(filePath.c_str(), &largura, &altura, &canais, 4);

	if (imgData)
	{
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, largura, altura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		//funcoes para parametrizar conforme o tamanho da imagem
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		
		stbi_image_free(imgData);
	}
	else {
		std::cout << "ERRO:: Nao foi possivel carregar a textura!" << filePath.c_str() << std::endl;
	}
}

void Textura::Bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Textura::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}