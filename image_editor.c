// # Copyright Stamatin Teodor 315CA 2022 - 2023
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "function.h"
#define LENGHT 100

int main()
{
    int ok = 1, loaded_image = 0, selected_image = 0, rgb = 0;
    char text[LENGHT], command[LENGHT], type[LENGHT];
    FILE *file;
    int **loaded1 = NULL, **loaded2 = NULL;
    int **matrixylw1 = NULL, **matrixblue1 = NULL, **matrixred1 = NULL;
    int **matrixylw2 = NULL, **matrixblue2 = NULL, **matrixred2 = NULL;
    int height1 = 1, width1 = 1, height2 = 1, width2 = 1, max_val;
    int x1, x2, y1, y2;
    while (ok == 1) {
        fgets(text, LENGHT, stdin);
        if (sscanf(text, "%s", command)) {
            if(strcmp(command, "LOAD") == 0) {
                if(loaded_image == 1) {
                    if(strstr("P3 P6", type) != NULL) {
                        free_matrix(height1, matrixred1);
                        free_matrix(height1, matrixylw1);
                        free_matrix(height1, matrixblue1);
                        if(selected_image == 1) {
                            free_matrix(height2, matrixred2);
                            free_matrix(height2, matrixylw2);
                            free_matrix(height2, matrixblue2);
                            selected_image = 0;
                        }
                    } else {
                        free_matrix(height1, loaded1);
                        if(selected_image == 1) {
                            free_matrix(height2, loaded2);
                            selected_image = 0;
                        }
                    }
                    loaded_image = 0;
                }
                char filename[LENGHT];
                sscanf(text, "%*s %s", filename);
                file = fopen(filename, "r");
                if (file == NULL) {
                    printf("Failed to load %s\n", filename);
                    loaded_image = 0;
                    continue;
                }
                char line[LENGHT];
                fgets(type, LENGHT, file);
                type[2]='\0';
                while(fgets(line, LENGHT, file) != NULL) {
                    if (line[0] != '#') {
                        break;
                    }
                }
                sscanf(line, "%d %d", &width1, &height1);
                fscanf(file, "%d", &max_val);
                if(strcmp(type, "P2") == 0) {
                    int **matrix = alloc_matrix(height1, width1);
                    for(int i = 0; i < height1; i++)
                        for(int j = 0; j < width1; j++)
                            fscanf(file, "%d", &matrix[i][j]);
                    loaded1 = matrix;
                    rgb = 0;
                }
                if(strcmp(type, "P3") == 0) {
                    int **matrixtmp1 = alloc_matrix(height1, width1);
                    int **matrixtmp2 = alloc_matrix(height1, width1);
                    int **matrixtmp3 = alloc_matrix(height1, width1);
                    for(int i = 0; i < height1; i++)
                        for(int j = 0; j < width1; j++) {
                            fscanf(file, "%d", &matrixtmp1[i][j]);
                            fscanf(file, "%d", &matrixtmp2[i][j]);
                            fscanf(file, "%d", &matrixtmp3[i][j]);
                        }
                    matrixred1 = matrixtmp1;
                    matrixylw1 = matrixtmp2;
                    matrixblue1 = matrixtmp3;
                    rgb = 1;
                }
                if(strcmp(type, "P5") == 0) {
                    int **matrix = alloc_matrix(height1, width1);
                    unsigned char x;
                    fread(&x, sizeof(x), 1, file);
                    for(int i = 0; i < height1; i++)
                        for(int j = 0; j < width1; j++) {
                            fread(&x, sizeof(x), 1, file);
                            matrix[i][j] = x;
                        }
                    loaded1 = matrix;
                    rgb = 0;
                }
                if(strcmp(type, "P6") == 0) {
                    int **matrixtmp1 = alloc_matrix(height1, width1);
                    int **matrixtmp2 = alloc_matrix(height1, width1);
                    int **matrixtmp3 = alloc_matrix(height1, width1);
                    unsigned char x;
                    fread(&x, sizeof(x), 1, file);
                    for(int i = 0; i < height1; i++)
                        for(int j = 0; j < width1; j++) {
                            fread(&x, sizeof(x), 1, file);
                            matrixtmp1[i][j] = x;
                            fread(&x, sizeof(x), 1, file);
                            matrixtmp2[i][j] = x;
                            fread(&x, sizeof(x), 1, file);
                            matrixtmp3[i][j] = x;
                        }
                    matrixred1 = matrixtmp1;
                    matrixylw1 = matrixtmp2;
                    matrixblue1 = matrixtmp3;
                    rgb = 1;
                }
                loaded_image = 1;
                fclose(file);
                printf("Loaded %s\n", filename);
            } else if (strcmp(command, "SELECT") == 0) {
                if(loaded_image == 0) {
                    printf("No image loaded\n");
                } else {
                    char select[LENGHT];
                    sscanf(text, "%*s %s", select);
                    if(strcmp(select, "ALL") == 0) {
                        if(rgb == 1) {
                            if(selected_image == 1) {
                                free_matrix(height1, matrixred1);
                                free_matrix(height1, matrixylw1);
                                free_matrix(height1, matrixblue1);
                                matrixred1 = matrixred2;
                                matrixylw1 = matrixylw2;
                                matrixblue1 = matrixblue2;
                                height1 = height2; width1 = width2;
                                height2 = 0; width2 = 0;
                            }
                        } else {
                            if(selected_image == 1) {
                                free_matrix(height1, loaded1);
                                loaded1 = loaded2;
                                height1 = height2; width1 = width2;
                                height2 = 0; width2 = 0;
                            }
                        }
                        printf("Selected ALL\n");
                        selected_image = 0;
                    } else {
                        int x1_aux, x2_aux, y1_aux, y2_aux;
                        if(sscanf(text, "%*s %d %d %d %d", &x1_aux, &y1_aux, &x2_aux, &y2_aux) != 4) {
                            printf("Invalid command\n");
                            continue;
                        }
                        if(selected_image == 0) {
                            if(x1_aux < 0 || x2_aux < 0 || y1_aux < 0 || y2_aux < 0 || x1_aux > width1 || x2_aux > width1 || y1_aux > height1 || y2_aux > height1) {
                                printf("Invalid set of coordinates\n");
                                continue;
                            }
                        } else {
                            if(x1_aux < 0 || x2_aux < 0 || y1_aux < 0 || y2_aux < 0 || x1_aux > width2 || x2_aux > width2 || y1_aux > height2 || y2_aux > height2) {
                                printf("Invalid set of coordinates\n");
                                continue;
                            }
                        }
                        if(x1_aux == x2_aux || y1_aux == y2_aux) {
                            printf("Invalid set of coordinates\n");
                            continue;
                        }
                        x1 = x1_aux; x2 = x2_aux; y1 = y1_aux; y2 = y2_aux;
                        int aux;
                        if(x1 > x2) {
                            aux = x1;
                            x1 = x2;
                            x2 = aux;
                        }
                        if(y1 > y2) {
                            aux = y1;
                            y1 = y2;
                            y2 = aux;
                        }
                        if(strstr("P3 P6", type)) {
                            if(selected_image == 1) {
                                int **matrixtmp1 = alloc_matrix(y2 - y1, x2 - x1);
                                int **matrixtmp2 = alloc_matrix(y2 - y1, x2 - x1);
                                int **matrixtmp3 = alloc_matrix(y2 - y1, x2 - x1);
                                for(int i = 0; i < y2 - y1; i++)
                                    for(int j = 0; j < x2 - x1; j++) {
                                        matrixtmp1[i][j] = matrixred2[i + y1][j + x1];
                                        matrixtmp2[i][j] = matrixylw2[i + y1][j + x1];
                                        matrixtmp3[i][j] = matrixblue2[i + y1][j + x1];
                                    }
                                free_matrix(height1, matrixred1);
                                free_matrix(height1, matrixylw1);
                                free_matrix(height1, matrixblue1);
                                matrixred1 = matrixtmp1;
                                matrixylw1 = matrixtmp2;
                                matrixblue1 = matrixtmp3;
                                height1 = y2 - y1;
                                width1 = x2 - x1;
                            }
                            else {
                                int **matrixtmp1 = alloc_matrix(y2 - y1, x2 - x1);
                                int **matrixtmp2 = alloc_matrix(y2 - y1, x2 - x1);
                                int **matrixtmp3 = alloc_matrix(y2 - y1, x2 - x1);
                                for(int i = 0; i < y2 - y1; i++)
                                    for(int j = 0; j < x2 - x1; j++) {
                                        matrixtmp1[i][j] = matrixred1[i + y1][j + x1];
                                        matrixtmp2[i][j] = matrixylw1[i + y1][j + x1];
                                        matrixtmp3[i][j] = matrixblue1[i + y1][j + x1];
                                    }
                                matrixred2 = matrixred1;
                                matrixred1 = matrixtmp1;
                                matrixylw2 = matrixylw1;
                                matrixylw1 = matrixtmp2;
                                matrixblue2 = matrixblue1;
                                matrixblue1 = matrixtmp3;
                                height2 = height1;
                                height1 = y2 - y1;
                                width2 = width1;
                                width1 = x2 - x1;
                                selected_image = 1;
                            }

                        } else {
                            int **matrix = alloc_matrix(y2 - y1, x2 - x1);
                            if(selected_image == 0)  {
                                for(int i = 0; i < y2 - y1; i++)
                                    for(int j = 0; j < x2 - x1; j++) {
                                        matrix[i][j] = loaded1[i + y1][j + x1];
                                    }
                                loaded2 = loaded1;
                                loaded1 = matrix;
                                height2 = height1;
                                height1 = y2 - y1;
                                width2 = width1;
                                width1 = x2 - x1;
                                selected_image = 1;
                            } else {
                                for(int i = 0; i < y2 - y1; i++)
                                    for(int j = 0; j < x2 - x1; j++) {
                                        matrix[i][j] = loaded2[i + y1][j + x1];
                                    }
                                free_matrix(height1, loaded1);
                                loaded1 = matrix;
                                height1 = y2 - y1;
                                width1 = x2 - x1;
                            }
                        }
                        printf("Selected %d %d %d %d\n", x1, y1, x2, y2);
                        selected_image = 1;
                    }
                }
            } else if (strcmp(command, "HISTOGRAM") == 0) {
                if(loaded_image == 0) {
                        printf("No image loaded\n");
                        continue;
                    }
                else {
                    int x, y, max_freq = 0, aux, nr;
                    nr = sscanf(text, "%*s %d %d %d", &x, &y, &aux);
                    if(nr != 2) {
                        printf("Invalid command\n");
                        continue;
                    } else {
                        if(strstr("P2 P5", type) == NULL) {
                            printf("Black and white image needed\n");
                            continue;
                        } else {
                        int *bin = (int *)calloc(y, sizeof(int));
                        for(int i = 0; i < height1; i++) {
                            for(int j = 0; j < width1; j++) {
                                bin[loaded1[i][j] * y / 256]++;
                                if(bin[loaded1[i][j] * y / 256] > max_freq)
                                    max_freq = bin[loaded1[i][j] * y / 256];
                            }
                        }
                        for(int i = 0; i < y; i++) {
                            bin[i] = (bin[i] * x) / max_freq;
                            printf("%d\t|\t ", bin[i]);
                            while(bin[i] > 0) {
                                printf("*");
                                bin[i]--;
                            }
                            printf("\n");
                        }
                        free(bin);
                        }
                    }
                }
            } else if(strcmp(command, "EQUALIZE") == 0) {
                if(loaded_image == 0)
                    printf("No image loaded\n");
                    else
                        if(strstr("P2 P5", type) == NULL)
                            printf("Black and white image needed\n");
                            else {
                                if(selected_image == 0) {
                                    double area = width1 * height1;
                                    int frequency[256] = {0};
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            frequency[loaded1[i][j]]++;
                                        }
                                    }
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            double x = 0;
                                            for(int k = 0; k <= loaded1[i][j]; k++) {
                                                x = x + frequency[k];
                                            }
                                            x = (x * 255) / (area * 1.0);
                                            x = round(x);
                                            if (x < 0)
                                                x = 0;
                                            else if (x > 255)
                                                    x = 255;
                                            loaded1[i][j] = x;
                                        }
                                    }
                                } else {
                                    double area = width2 * height2;
                                    int frequency[256] = {0};
                                    for(int i = 0; i < height2; i++) {
                                        for(int j = 0; j < width2; j++) {
                                            frequency[loaded2[i][j]]++;
                                        }
                                    }
                                    for(int i = 0; i < height2; i++) {
                                        for(int j = 0; j < width2; j++) {
                                            double x = 0;
                                            for(int k = 0; k <= loaded2[i][j]; k++) {
                                                x = x + frequency[k];
                                            }
                                            x = (x * 255) / (area * 1.0);
                                            x = round(x);
                                            if (x < 0)
                                                x = 0;
                                            else if (x > 255)
                                                    x = 255;
                                            loaded2[i][j] = x;
                                        }
                                    }
                                    for(int i = 0; i < height1; i++)
                                        for(int j = 0; j < width1; j++) {
                                            loaded1[i][j] = loaded2[i + y1][j + x1];
                                    }
                                }
                                printf("Equalize done\n");
                            }
            } else if(strcmp(command, "CROP") == 0) {
                if(loaded_image == 0)
                    printf("No image loaded\n");
                    else {
                        if(selected_image == 1) {
                            if(rgb == 1) {
                                free_matrix(height2, matrixred2);
                                free_matrix(height2, matrixylw2);
                                free_matrix(height2, matrixblue2);
                                height2 = 0; width2 = 0;
                                selected_image = 0;
                            } else {
                                free_matrix(height2, loaded2);
                                height2 = 0; width2 = 0;
                                selected_image = 0;
                            }
                        }
                        printf("Image cropped\n");
                    }
            } else if (strcmp(command, "APPLY") == 0) {
                if(loaded_image == 0)
                    printf("No image loaded\n");
                    else {
                        char parameter[LENGHT];
                        int nr = sscanf(text, "%*s %s", parameter);
                        if(!(strcmp(parameter, "EDGE") == 0 || strcmp(parameter, "SHARPEN") == 0 || strcmp(parameter, "BLUR") == 0 || strcmp(parameter, "GAUSSIAN_BLUR") == 0)) {
                            if(nr > 0) {
                                printf("APPLY parameter invalid\n");
                                continue;
                            } else {
                                printf("Invalid command\n");
                                continue;
                            }
                        } else { 
                            if(strstr("P2 P5", type) != NULL) {
                                printf("Easy, Charlie Chaplin\n");
                                continue;
                            } else {
                                if (strcmp(parameter, "EDGE") == 0) {
                                    int **matrixtmp1 = alloc_matrix(height1, width1);
                                    int **matrixtmp2 = alloc_matrix(height1, width1);
                                    int **matrixtmp3 = alloc_matrix(height1, width1);
                                    int kernel_edge[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
                                    if(selected_image == 0) {
                                        for(int i = 0; i < height1; i++) {
                                            for(int j = 0; j < width1; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height1 - 1 || j == width1 - 1) {
                                                    matrixtmp1[i][j] = matrixred1[i][j];
                                                    matrixtmp2[i][j] = matrixylw1[i][j];
                                                    matrixtmp3[i][j] = matrixblue1[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) { 
                                                            sum1 = sum1 + kernel_edge[ki][kj] * matrixred1[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_edge[ki][kj] * matrixylw1[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_edge[ki][kj] * matrixblue1[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i][j] = round(sum1);
                                                    matrixtmp2[i][j] = round(sum2);
                                                    matrixtmp3[i][j] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    } else {
                                        for(int i = y1; i < y2; i++) {
                                            for(int j = x1; j < x2; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height2 - 1 || j == width2 - 1) {
                                                    matrixtmp1[i - y1][j - x1] = matrixred2[i][j];
                                                    matrixtmp2[i - y1][j - x1] = matrixylw2[i][j];
                                                    matrixtmp3[i - y1][j - x1] = matrixblue2[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) {
                                                            sum1 = sum1 + kernel_edge[ki][kj] * matrixred2[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_edge[ki][kj] * matrixylw2[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_edge[ki][kj] * matrixblue2[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i - y1][j - x1] = round(sum1);
                                                    matrixtmp2[i - y1][j - x1] = round(sum2);
                                                    matrixtmp3[i - y1][j - x1] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                                matrixred2[i + y1][j + x1] = matrixtmp1[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                                matrixylw2[i + y1][j + x1] = matrixtmp2[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                                matrixblue2[i + y1][j + x1] = matrixtmp3[i][j];
                                            }
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    }
                                } else if (strcmp(parameter, "SHARPEN") == 0) {
                                    int **matrixtmp1 = alloc_matrix(height1, width1);
                                    int **matrixtmp2 = alloc_matrix(height1, width1);
                                    int **matrixtmp3 = alloc_matrix(height1, width1);
                                    int kernel_sharpen[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
                                    if(selected_image == 0) {
                                        for(int i = 0; i < height1; i++) {
                                            for(int j = 0; j < width1; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height1 - 1 || j == width1 - 1) {
                                                    matrixtmp1[i][j] = matrixred1[i][j];
                                                    matrixtmp2[i][j] = matrixylw1[i][j];
                                                    matrixtmp3[i][j] = matrixblue1[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) {
                                                            sum1 = sum1 + kernel_sharpen[ki][kj] * matrixred1[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_sharpen[ki][kj] * matrixylw1[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_sharpen[ki][kj] * matrixblue1[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i][j] = round(sum1);
                                                    matrixtmp2[i][j] = round(sum2);
                                                    matrixtmp3[i][j] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    } else {
                                        for(int i = y1; i < y2; i++) {
                                            for(int j = x1; j < x2; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height2 - 1 || j == width2 - 1) {
                                                    matrixtmp1[i - y1][j - x1] = matrixred2[i][j];
                                                    matrixtmp2[i - y1][j - x1] = matrixylw2[i][j];
                                                    matrixtmp3[i - y1][j - x1] = matrixblue2[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) {
                                                            sum1 = sum1 + kernel_sharpen[ki][kj] * matrixred2[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_sharpen[ki][kj] * matrixylw2[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_sharpen[ki][kj] * matrixblue2[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i - y1][j - x1] = round(sum1);
                                                    matrixtmp2[i - y1][j - x1] = round(sum2);
                                                    matrixtmp3[i - y1][j - x1] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                                matrixred2[i + y1][j + x1] = matrixtmp1[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                                matrixylw2[i + y1][j + x1] = matrixtmp2[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                                matrixblue2[i + y1][j + x1] = matrixtmp3[i][j];
                                            }
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    }
                                } else if (strcmp(parameter, "BLUR") == 0) {
                                    int **matrixtmp1 = alloc_matrix(height1, width1);
                                    int **matrixtmp2 = alloc_matrix(height1, width1);
                                    int **matrixtmp3 = alloc_matrix(height1, width1);
                                    double kernel_blur[3][3] = {{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}, {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}, {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}};
                                    if(selected_image == 0) {
                                        for(int i = 0; i < height1; i++) {
                                            for(int j = 0; j < width1; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height1 - 1 || j == width1 - 1) {
                                                    matrixtmp1[i][j] = matrixred1[i][j];
                                                    matrixtmp2[i][j] = matrixylw1[i][j];
                                                    matrixtmp3[i][j] = matrixblue1[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) {
                                                            sum1 = sum1 + kernel_blur[ki][kj] * matrixred1[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_blur[ki][kj] * matrixylw1[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_blur[ki][kj] * matrixblue1[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i][j] = round(sum1);
                                                    matrixtmp2[i][j] = round(sum2);
                                                    matrixtmp3[i][j] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    } else {
                                        for(int i = y1; i < y2; i++) {
                                            for(int j = x1; j < x2; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height2 - 1 || j == width2 - 1) {
                                                    matrixtmp1[i - y1][j - x1] = matrixred2[i][j];
                                                    matrixtmp2[i - y1][j - x1] = matrixylw2[i][j];
                                                    matrixtmp3[i - y1][j - x1] = matrixblue2[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) {
                                                            sum1 = sum1 + kernel_blur[ki][kj] * matrixred2[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_blur[ki][kj] * matrixylw2[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_blur[ki][kj] * matrixblue2[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i - y1][j - x1] = round(sum1);
                                                    matrixtmp2[i - y1][j - x1] = round(sum2);
                                                    matrixtmp3[i - y1][j - x1] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                                matrixred2[i + y1][j + x1] = matrixtmp1[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                                matrixylw2[i + y1][j + x1] = matrixtmp2[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                                matrixblue2[i + y1][j + x1] = matrixtmp3[i][j];
                                            }
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    }
                                } else if (strcmp(parameter, "GAUSSIAN_BLUR") == 0) {
                                    int **matrixtmp1 = alloc_matrix(height1, width1);
                                    int **matrixtmp2 = alloc_matrix(height1, width1);
                                    int **matrixtmp3 = alloc_matrix(height1, width1);
                                    double kernel_blur[3][3] = {{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}, {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0}, {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}};
                                    if(selected_image == 0) {
                                        for(int i = 0; i < height1; i++) {
                                            for(int j = 0; j < width1; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height1 - 1 || j == width1 - 1) {
                                                    matrixtmp1[i][j] = matrixred1[i][j];
                                                    matrixtmp2[i][j] = matrixylw1[i][j];
                                                    matrixtmp3[i][j] = matrixblue1[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) {
                                                            sum1 = sum1 + kernel_blur[ki][kj] * matrixred1[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_blur[ki][kj] * matrixylw1[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_blur[ki][kj] * matrixblue1[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i][j] = round(sum1);
                                                    matrixtmp2[i][j] = round(sum2);
                                                    matrixtmp3[i][j] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++)
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    } else {
                                        for(int i = y1; i < y2; i++) {
                                            for(int j = x1; j < x2; j++) {
                                                double sum1 = 0, sum2 = 0, sum3 = 0, sw = 0;
                                                if(i == 0 || j == 0 || i == height2 - 1 || j == width2 - 1) {
                                                    matrixtmp1[i - y1][j - x1] = matrixred2[i][j];
                                                    matrixtmp2[i - y1][j - x1] = matrixylw2[i][j];
                                                    matrixtmp3[i - y1][j - x1] = matrixblue2[i][j];
                                                } else {
                                                    for(int ki = 0; ki < 3; ki++)
                                                        for(int kj = 0; kj < 3; kj++) {
                                                            sum1 = sum1 + kernel_blur[ki][kj] * matrixred2[i - 1 + ki][j - 1 + kj];
                                                            sum2 = sum2 + kernel_blur[ki][kj] * matrixylw2[i - 1 + ki][j - 1 + kj];
                                                            sum3 = sum3 + kernel_blur[ki][kj] * matrixblue2[i - 1 + ki][j - 1 + kj];
                                                            sw = 1;
                                                        }
                                                }
                                                if(sw == 1) {
                                                    if(sum1 > 255)
                                                    sum1 = 255;
                                                    if(sum1 < 0)
                                                        sum1 = 0;
                                                    if(sum2 > 255)
                                                        sum2 = 255;
                                                    if(sum2 < 0)
                                                        sum2 = 0;
                                                    if(sum3 > 255)
                                                        sum3 = 255;
                                                    if(sum3 < 0)
                                                        sum3 = 0;
                                                    matrixtmp1[i - y1][j - x1] = round(sum1);
                                                    matrixtmp2[i - y1][j - x1] = round(sum2);
                                                    matrixtmp3[i - y1][j - x1] = round(sum3);
                                                }
                                            }
                                        }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixred1[i][j] = matrixtmp1[i][j];
                                                matrixred2[i + y1][j + x1] = matrixtmp1[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixylw1[i][j] = matrixtmp2[i][j];
                                                matrixylw2[i + y1][j + x1] = matrixtmp2[i][j];
                                            }
                                        for(int i = 0; i < height1; i++)
                                            for(int j = 0; j < width1; j++) {
                                                matrixblue1[i][j] = matrixtmp3[i][j];
                                                matrixblue2[i + y1][j + x1] = matrixtmp3[i][j];
                                            }
                                        free_matrix(height1, matrixtmp1);
                                        free_matrix(height1, matrixtmp2);
                                        free_matrix(height1, matrixtmp3);
                                    }
                                }
                            }
                            printf("APPLY %s done\n", parameter);
                        }
                    }
            } else if (strcmp(command, "SAVE") == 0) {
                if (loaded_image == 0) {
                    printf("No image loaded\n");
                }
                else {
                    char filename_save[LENGHT], save_type[LENGHT];
                    int nr = sscanf(text, "%*s %s %s", filename_save, save_type);
                    FILE *file_save = fopen(filename_save, "w");
                    if(nr == 2) {
                        if(strcmp(save_type, "ascii") == 0) {
                            if(strcmp(type, "P5") == 0) {
                                strcpy(type, "P2");
                            }
                            if(strcmp(type, "P6") == 0) {
                                strcpy(type, "P3");
                            }
                            if(selected_image == 0) {
                                if(strcmp(type, "P2") == 0) {
                                    fprintf(file_save, "%s\n%d %d\n%d\n", type, width1, height1, max_val);
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            fprintf(file_save, "%d ", loaded1[i][j]);
                                        }
                                        fprintf(file_save, "\n");
                                    }
                                }
                                if(strcmp(type, "P3") == 0) {
                                    fprintf(file_save, "%s\n%d %d\n%d\n", type, width1, height1, max_val);
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            fprintf(file_save, "%d ", matrixred1[i][j]);
                                            fprintf(file_save, "%d ", matrixylw1[i][j]);
                                            fprintf(file_save, "%d ", matrixblue1[i][j]);
                                        }
                                        fprintf(file_save, "\n");
                                    }
                                }
                            } else {
                                if(strcmp(type, "P2") == 0) {
                                    fprintf(file_save, "%s\n%d %d\n%d\n", type, width2, height2, max_val);
                                    for(int i = 0; i < height2; i++) {
                                        for(int j = 0; j < width2; j++) {
                                            fprintf(file_save, "%d ", loaded2[i][j]);
                                        }
                                        fprintf(file_save, "\n");
                                    }
                                }
                                if(strcmp(type, "P3") == 0) {
                                    fprintf(file_save, "%s\n%d %d\n%d\n", type, width2, height2, max_val);
                                    for(int i = 0; i < height2; i++) {
                                        for(int j = 0; j < width2; j++) {
                                            fprintf(file_save, "%d ", matrixred2[i][j]);
                                            fprintf(file_save, "%d ", matrixylw2[i][j]);
                                            fprintf(file_save, "%d ", matrixblue2[i][j]);
                                        }
                                        fprintf(file_save, "\n");
                                    }
                                }
                            }
                            fclose(file_save);
                        }
                    } else if(nr == 1) {
                        if(strcmp(type, "P2") == 0) {
                            strcpy(type, "P5");
                        }
                        if(strcmp(type, "P3") == 0) {
                            strcpy(type, "P6");
                        }
                        if(selected_image == 0) {
                            if(strcmp(type, "P5") == 0) {
                                fprintf(file_save, "%s\n%d %d\n%d\n", type, width1, height1, max_val);
                                for(int i = 0; i < height1; i++) {
                                    for(int j = 0; j < width1; j++) {
                                        fwrite(&loaded1[i][j], sizeof(unsigned char), 1, file_save);
                                    }
                                }
                            }
                            if(strcmp(type, "P6") == 0) {
                                fprintf(file_save, "%s\n%d %d\n%d\n", type, width1, height1, max_val);
                                for(int i = 0; i < height1; i++) {
                                    for(int j = 0; j < width1; j++) {
                                        fwrite(&matrixred1[i][j], sizeof(unsigned char), 1, file_save);
                                        fwrite(&matrixylw1[i][j], sizeof(unsigned char), 1, file_save);
                                        fwrite(&matrixblue1[i][j], sizeof(unsigned char), 1, file_save);
                                    }
                                }
                            }
                        } else {
                            if(strcmp(type, "P5") == 0) {
                                fprintf(file_save, "%s\n%d %d\n%d\n", type, width2, height2, max_val);
                                for(int i = 0; i < height2; i++) {
                                    for(int j = 0; j < width2; j++) {
                                        fwrite(&loaded2[i][j], sizeof(unsigned char), 1, file_save);
                                    }
                                }
                            }
                            if(strcmp(type, "P6") == 0) {
                                fprintf(file_save, "%s\n%d %d\n%d\n", type, width2, height2, max_val);
                                for(int i = 0; i < height2; i++) {
                                    for(int j = 0; j < width2; j++) {
                                        fwrite(&matrixred2[i][j], sizeof(unsigned char), 1, file_save);
                                        fwrite(&matrixylw2[i][j], sizeof(unsigned char), 1, file_save);
                                        fwrite(&matrixblue2[i][j], sizeof(unsigned char), 1, file_save);
                                    }
                                }
                            }
                        }
                        fclose(file_save);
                    }
                    printf("Saved %s\n", filename_save);
                }
            } else if (strcmp(command, "EXIT") == 0) {
                ok = 0;
                if(loaded_image == 0) {
                    printf("No image loaded\n");
                    continue;
                }
                if(selected_image == 0) {
                    if(rgb == 0) {
                        free_matrix(height1, loaded1);
                    } else {
                        free_matrix(height1, matrixred1);
                        free_matrix(height1, matrixylw1);
                        free_matrix(height1, matrixblue1);
                    }
                } else {
                    if(rgb == 0) {
                        free_matrix(height1, loaded1);
                        free_matrix(height2, loaded2);
                    } else {
                        free_matrix(height1, matrixred1);
                        free_matrix(height1, matrixylw1);
                        free_matrix(height1, matrixblue1);
                        free_matrix(height2, matrixred2);
                        free_matrix(height2, matrixylw2);
                        free_matrix(height2, matrixblue2);
                    }
                }
            } else if(strcmp(command, "ROTATE") == 0) {
                if(loaded_image == 0) {
                    printf("No image loaded\n");
                } else {
                    int angle;
                    sscanf(text, "%*s %d", &angle);
                    if(selected_image == 0 || (height2 == height1 && width2 == width1)) {
                        if(!(angle == 90 || angle == -90 || angle == 180 || angle == -180 || angle == 270 || angle == -270 || angle == 360 || angle == -360 || angle == 0)) {
                            printf("Unsupported rotation angle\n");
                        } else {
                            if(angle >= 0) {
                                for(int count = 0; count < (angle / 90); count++) {
                                    if(rgb == 0) {
                                        int **aux = alloc_matrix(width1, height1);
                                        for (int i = 0; i < height1; i++) {
                                            for (int j = 0; j < width1; j++) {
                                                aux[j][height1 - i - 1] = loaded1[i][j];
                                            }
                                        }
                                        free_matrix(height1, loaded1);
                                        loaded1 = aux;
                                        int tmp = height1;
                                        height1 = width1;
                                        width1 = tmp;
                                    } else {
                                        int **auxred = alloc_matrix(width1, height1);
                                        int **auxylw = alloc_matrix(width1, height1);
                                        int **auxblue = alloc_matrix(width1, height1);
                                        for (int i = 0; i < height1; i++) {
                                            for (int j = 0; j < width1; j++) {
                                                auxred[j][height1 - i - 1] = matrixred1[i][j];
                                                auxylw[j][height1 - i - 1] = matrixylw1[i][j];
                                                auxblue[j][height1 - i - 1] = matrixblue1[i][j];
                                            }
                                        }
                                        free_matrix(height1, matrixred1);
                                        free_matrix(height1, matrixylw1);
                                        free_matrix(height1, matrixblue1);
                                        matrixred1 = auxred;
                                        matrixylw1 = auxylw;
                                        matrixblue1 = auxblue;
                                        int tmp = height1;
                                        height1 = width1;
                                        width1 = tmp;
                                    }
                                }
                            } else {
                                for(int count = 0; count > (angle / 90); count--) {
                                    if(rgb == 0) {
                                        int **aux = alloc_matrix(width1, height1);
                                        for (int i = 0; i < height1; i++) {
                                            for (int j = 0; j < width1; j++) {
                                                aux[width1 - 1 - j][i] = loaded1[i][j];
                                            }
                                        }
                                        free_matrix(height1, loaded1);
                                        loaded1 = aux;
                                        int tmp = height1;
                                        height1 = width1;
                                        width1 = tmp;
                                    } else {
                                        int **auxred = alloc_matrix(width1, height1);
                                        int **auxylw = alloc_matrix(width1, height1);
                                        int **auxblue = alloc_matrix(width1, height1);
                                        for (int i = 0; i < height1; i++) {
                                            for (int j = 0; j < width1; j++) {
                                                auxred[width1 - 1 - j][i] = matrixred1[i][j];
                                                auxylw[width1 - 1 - j][i] = matrixylw1[i][j];
                                                auxblue[width1 - 1 - j][i] = matrixblue1[i][j];
                                            }
                                        }
                                        free_matrix(height1, matrixred1);
                                        free_matrix(height1, matrixylw1);
                                        free_matrix(height1, matrixblue1);
                                        matrixred1 = auxred;
                                        matrixylw1 = auxylw;
                                        matrixblue1 = auxblue;
                                        int tmp = height1;
                                        height1 = width1;
                                        width1 = tmp;
                                    }
                                }
                            }
                            if(selected_image == 1) {
                                if(rgb == 1) {
                                    int **matred = alloc_matrix(height1, width1);
                                    int **matylw = alloc_matrix(height1, width1);
                                    int **matblue = alloc_matrix(height1, width1);
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            matred[i][j] = matrixred1[i][j];
                                            matylw[i][j] = matrixylw1[i][j];
                                            matblue[i][j] = matrixblue1[i][j];
                                        }
                                    }
                                    free_matrix(height2, matrixred2);
                                    free_matrix(height2, matrixylw2);
                                    free_matrix(height2, matrixblue2);
                                    matrixred2 = matred;
                                    matrixylw2 = matylw;
                                    matrixblue2 = matblue;
                                    height2 = height1;
                                    width2 = width1;
                                } else {
                                    free_matrix(height2, loaded2);
                                    loaded2 = alloc_matrix(height1, width1);
                                    for(int i = 0; i < height1; i++)
                                        for(int j = 0; j < width1; j++)
                                            loaded2[i][j] = loaded1[i][j];
                                    height2 = height1;
                                    width2 = width1;
                                }
                            }
                            printf("Rotated %d\n", angle);
                        }
                    } else {
                        if(!(angle == 90 || angle == -90 || angle == 180 || angle == -180 || angle == 270 || angle == -270 || angle == 360 || angle == -360 || angle == 0)) {
                            printf("Unsupported rotation angle\n");
                        } else {
                            if(height1 != width1) {
                                printf("The selection must be square\n");
                                continue;
                            } else {
                                if(angle >= 0) {
                                    for(int count = 0; count < (angle / 90); count++) {
                                        if(rgb == 0) {
                                            int **aux = alloc_matrix(width1, height1);
                                            for (int i = 0; i < height1; i++) {
                                                for (int j = 0; j < width1; j++) {
                                                    aux[j][height1 - i - 1] = loaded1[i][j];
                                                }
                                            }
                                            free_matrix(height1, loaded1);
                                            loaded1 = aux;
                                        } else {
                                            int **auxred = alloc_matrix(width1, height1);
                                            int **auxylw = alloc_matrix(width1, height1);
                                            int **auxblue = alloc_matrix(width1, height1);
                                            for (int i = 0; i < height1; i++) {
                                                for (int j = 0; j < width1; j++) {
                                                    auxred[j][height1 - i - 1] = matrixred1[i][j];
                                                    auxylw[j][height1 - i - 1] = matrixylw1[i][j];
                                                    auxblue[j][height1 - i - 1] = matrixblue1[i][j];
                                                }
                                            }
                                            free_matrix(height1, matrixred1);
                                            free_matrix(height1, matrixylw1);
                                            free_matrix(height1, matrixblue1);
                                            matrixred1 = auxred;
                                            matrixylw1 = auxylw;
                                            matrixblue1 = auxblue;
                                        }
                                    }
                                } else {
                                    for(int count = 0; count > (angle / 90); count--) {
                                        if(rgb == 0) {
                                            int **aux = alloc_matrix(width1, height1);
                                            for (int i = 0; i < height1; i++) {
                                                for (int j = 0; j < width1; j++) {
                                                    aux[width1 - 1 - j][i] = loaded1[i][j];
                                                }
                                            }
                                            free_matrix(height1, loaded1);
                                            loaded1 = aux;
                                        } else {
                                            int **auxred = alloc_matrix(width1, height1);
                                            int **auxylw = alloc_matrix(width1, height1);
                                            int **auxblue = alloc_matrix(width1, height1);
                                            for (int i = 0; i < height1; i++) {
                                                for (int j = 0; j < width1; j++) {
                                                    auxred[width1 - 1 - j][i] = matrixred1[i][j];
                                                    auxylw[width1 - 1 - j][i] = matrixylw1[i][j];
                                                    auxblue[width1 - 1 - j][i] = matrixblue1[i][j];
                                                }
                                            }
                                            free_matrix(height1, matrixred1);
                                            free_matrix(height1, matrixylw1);
                                            free_matrix(height1, matrixblue1);
                                            matrixred1 = auxred;
                                            matrixylw1 = auxylw;
                                            matrixblue1 = auxblue;
                                        }
                                    }
                                }
                                if(rgb == 1) {
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            matrixred2[i + y1][j + x1] = matrixred1[i][j];
                                        }
                                    }
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            matrixylw2[i + y1][j + x1] = matrixylw1[i][j];
                                        }
                                    }
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            matrixblue2[i + y1][j + x1] = matrixblue1[i][j];
                                        }
                                    }
                                } else {
                                    for(int i = 0; i < height1; i++) {
                                        for(int j = 0; j < width1; j++) {
                                            loaded2[i + y1][j + x1] = loaded1[i][j];
                                        }
                                    }
                                }
                                printf("Rotated %d\n", angle);
                            }
                        }
                    }
                }
            } else {
                printf("Invalid command\n");
            }
        }
    }
    return 0;
}