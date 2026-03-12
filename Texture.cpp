#include "stdafx.h"
#include "Texture.h"

void Texture::Load(std::string file)
{

	if (this->tr->type == "2d") {
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		int width, height, bpp;
		char* array_pixeli = LoadTGA(file.c_str(), &width, &height, &bpp);
		if (bpp == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, array_pixeli);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, array_pixeli);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (this->tr->wrap_s == 33071) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		}
		else if (this->tr->wrap_s == 10497) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}

		if (this->tr->wrap_t == 33071) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else if (this->tr->wrap_t == 10497) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

	} else if (this->tr->type == "cube_map") {
	 	glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

		int width, height, bpp;
		char* array_pixeli = LoadTGA(file.c_str(), &width, &height, &bpp);

		int nr_bytes = bpp / 8;
		int mini_width = width / 4;
		int mini_height = height / 3;
		int h_index[6] = { 1, 1, 0, 2, 1, 1 }; // indexul reprezinta numarul ecranului
		int w_index[6] = { 2, 0, 1, 1, 1, 3 };

		char *array_fata = new char[mini_width * mini_height * nr_bytes];
		int index;
		for (index = 0; index < 6; index++) {
			int h = h_index[index] * mini_height;
			int w = w_index[index] * nr_bytes * mini_width;
			int pixels = 0;
			while (h < (h_index[index] + 1) * mini_height) {
				if (w < (w_index[index] + 1) * nr_bytes * mini_width) {
					array_fata[pixels++] = array_pixeli[h * width * nr_bytes + w];
					w++;
				} else {
					h++;
					w = w_index[index] * nr_bytes * mini_width;
				}
			}
			if (bpp == 32)
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGBA, mini_width, mini_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, array_fata);
			else
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB, mini_width, mini_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, array_fata);
		}
		delete[] array_fata;

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (this->tr->wrap_s == 33071) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		}
		else if (this->tr->wrap_s == 10497) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		}

		if (this->tr->wrap_t == 33071) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
		else if (this->tr->wrap_t == 10497) {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}