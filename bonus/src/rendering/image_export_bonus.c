#include "../../includes/miniRT_bonus.h"

bool	mlx_image_to_ppm(mlx_image_t *img, char *string, unsigned int range)
{
	FILE *output = fopen (string ,"w");
	if (!output)
		return false;
	fprintf(output, "P3\n");
	fprintf(output, "%i %i\n", img->width, img->height);
	fprintf(output, "%u\n", range);
	for (int i = 0; i < (int)(img->width * img->height * 4); i += 4)
	{
		uint8_t r = img->pixels[i + 0];
		uint8_t g = img->pixels[i + 1];
		uint8_t b = img->pixels[i + 2];

		fprintf(output, "%3u %3u %3u ", r, g, b);
		if ((i / 4 + 1) % img->width == 0)
			fprintf(output, "\n");
	}
		fprintf(output, "\n");
	fclose (output);
	return true;
}