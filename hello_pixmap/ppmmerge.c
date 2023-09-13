// GENERATED by ChatGPT 4.0 from OpenAI
//
// Prompted by Sam Siewert
//
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int width, height;
    Pixel *data;
} Image;

// Function to skip comments in PPM
void skip_comments(FILE *fp) {
    char ch = fgetc(fp);
    while (ch == '#') {
        while (fgetc(fp) != '\n');
        ch = fgetc(fp);
    }
    ungetc(ch, fp);
}

Image *load_ppm(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    char type[3];
    fscanf(file, "%2s", type);
    fgetc(file);  // To consume newline

    if (type[0] != 'P' || type[1] != '6') {
        fprintf(stderr, "Only P6 format is supported.\n");
        exit(1);
    }

    skip_comments(file);

    Image *img = (Image *)malloc(sizeof(Image));
    int max_value;

    fscanf(file, "%d %d", &img->width, &img->height);
    skip_comments(file);
    fscanf(file, "%d", &max_value);
    fgetc(file);  // Consume the newline after max_value

    img->data = (Pixel *)malloc(img->width * img->height * sizeof(Pixel));

    fread(img->data, sizeof(Pixel), img->width * img->height, file);

    fclose(file);
    return img;
}

void save_ppm(const char *filename, Image *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Failed to open file");
        exit(1);
    }

    fprintf(file, "P6\n%d %d\n255\n", img->width, img->height);

    fwrite(img->data, sizeof(Pixel), img->width * img->height, file);

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <larger_image.ppm> <smaller_image.ppm>\n", argv[0]);
        return 1;
    }

    Image *img1 = load_ppm(argv[1]);
    Image *img2 = load_ppm(argv[2]);

    if (img1->width < img2->width || img1->height < img2->height) {
        fprintf(stderr, "The first image should be larger than the second image.\n");
        return 1;
    }

    int offsetX = (img1->width - img2->width) / 2;
    int offsetY = (img1->height - img2->height) / 2;

    for (int i = 0; i < img2->height; i++) {
        for (int j = 0; j < img2->width; j++) {
            img1->data[(i + offsetY) * img1->width + (j + offsetX)] = img2->data[i * img2->width + j];
        }
    }

    save_ppm("hello-world.ppm", img1);

    free(img1->data);
    free(img1);
    free(img2->data);
    free(img2);

    return 0;
}