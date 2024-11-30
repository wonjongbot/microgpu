#include "../libs/tinyrenderer/tgaimage.h"
#include "../libs/tinyrenderer/model.h"
#include <iostream>

#define IPART(x) (std::floor(x))
#define ROUND(x) IPART(x + 0.5)
#define FPART(x) (x - IPART(x))
#define RFPART(x) (1 - FPART(x))
//#define BNESS(color) ((0.114f * color[2] + 0.587f * color[1] + 0.299f * color[0])/255)
#define BNESS(color) (1)


void linewu(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    bool steep = false;

    if (std::abs(x0 - x1) < std::abs(y0 - y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    }
    if (x0 > x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    float grad = 0;

    if (dx == 0)
        grad = 1.;
    else
        grad = dy/(float)dx;
    
    float orig_brightness = BNESS(color);

    float xend = ROUND((float)x0);
    float yend = y0 + grad * (xend - x0);
    float xgap = RFPART(x0 + 0.5);
    //std::cout<<(xgap)<<std::endl;
    float xpx1 = xend;
    float ypx1 = IPART(yend);

    if (steep){
        float scale = (RFPART(yend) * xgap) / orig_brightness;
        image.set(ypx1, xpx1, color * scale);

        scale = FPART(yend) * xgap / orig_brightness;;
        image.set(ypx1 + 1, xpx1, color * scale);
    }else{
        float scale = (RFPART(yend) * xgap) / orig_brightness;
        image.set(xpx1, ypx1, color*scale);

        scale = FPART(yend) * xgap / orig_brightness;
        image.set(xpx1 + 1, ypx1, color*scale);
    }
    float intery = yend + grad;

    xend = ROUND((float)x1);
    yend = y1 + grad * (xend - x1);
    xgap = FPART(x1 + 0.5);
    //std::cout<<(xgap)<<std::endl;
    float xpx2 = xend;
    float ypx2 = IPART(yend);

    float scale1 = (RFPART(yend) * xgap) / orig_brightness;
    float scale2 = (FPART(yend) * xgap) / orig_brightness;;

    if (steep){
        image.set(ypx2, xpx2, color*scale1);
        image.set(ypx2 + 1, xpx2, color*scale2);
    }else{
        image.set(xpx2, ypx2, color*scale1);
        image.set(xpx2 + 1, ypx2, color*scale2);
    }

    float scale;

    if (steep){
        for(int x = xpx1 + 1; x < xpx2; x++){
            scale = (RFPART(intery)) / orig_brightness;
            image.set(IPART(intery), x, color * scale);

            scale = (FPART(intery)) / orig_brightness;
            image.set(IPART(intery) + 1, x, color * scale);
            intery += grad;
        }
    } else{
        for(int x = xpx1 + 1; x < xpx2; x++){
            scale = (RFPART(intery)) * orig_brightness;
            scale = (RFPART(intery));
            image.set(x, IPART(intery), color * scale);

            scale = (FPART(intery)) * orig_brightness;
            scale = (FPART(intery));
            image.set(x, IPART(intery) + 1, color * scale);
            intery += grad;
        }
    }
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) { 
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    for (int x=x0; x<=x1; x++) { 
        if (steep) { 
            image.set(y, x, color); 
        } else { 
            image.set(x, y, color); 
        } 
        error2 += derror2; 
        if (error2 > dx) { 
            y += (y1>y0?1:-1); 
            error2 -= dx*2; 
        } 
    } 
}

void drawLine(vec2 t0, vec2 t1, TGAImage& image, TGAColor color){
    line(t0.x, t0.y, t1.x, t1.y, image, color);
}


const TGAColor white(255,255,255,255);
const TGAColor red(0,0,255,255);
const TGAColor green(0,255,0,255);
const TGAColor blue(255,0,0,255);


// void linetest(){
//     //image.set(52, 41, red);

//     //line(13, 20, 80, 40, image, white); 

//     linewu(20, 13, 40, 80, image, white); 
//     line(15, 18, 35, 85, image, white); 
//     linewu(85, 40, 13, 20, image, red);
//     line(85, 45, 13, 25, image, red);

//     linewu(0, 0, 100, 4, image, white);
//     linewu(0, 0, 4, 100, image, white);

//     //image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    
// }

void drawTriangle(vec2 t0, vec2 t1, vec2 t2, TGAImage &image, TGAColor color){
    // drawLine(t0, t1, image, white);
    // drawLine(t1, t2, image, white);
    // drawLine(t2, t0, image, white);

    /* sweeping algo */

    /*  t2
        *
           
      * t1
               * t0
    */
   
    // y value is increasingly large (t2 largest)
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t1.y > t2.y) std::swap(t1, t2);
    if (t0.y > t1.y) std::swap(t0, t1);

    //std::cout<<t0<<" "<<t1<<" "<<t2<<std::endl;

    float grad_t2_t1 = (t2.x - t1.x) / (float)(t2.y - t1.y);
    float grad_t2_t0 = (t2.x - t0.x) / (float)(t2.y - t0.y);
    float grad_t1_t0 = (t1.x - t0.x) / (float)(t1.y - t0.y);

    std::cout << "grad_t2_t1: " << grad_t2_t1 << ", grad_t2_t0: " << grad_t2_t0
          << ", grad_t1_t0: " << grad_t1_t0 << std::endl;

    //top half
    float x0 = t2.x;
    float x1 = t2.x;
    for(int y = t2.y; y > t1.y; y--){
        //std::cout<<"HI"<<y<<std::endl;
        line(x0, y, x1, y, image, color);
        x0 -= grad_t2_t1;
        x1 -= grad_t2_t0;
    }
    for(int y = t1.y; y > t0.y; y--){
        //std::cout<<"HI"<<y<<std::endl;
        line(x0, y, x1, y, image, color);
        x0 -= grad_t1_t0;
        x1 -= grad_t2_t0;
    }

}

void drawModelFace(Model*& model, TGAImage& image){
    for(int i = 0; i < model->nfaces(); i++){
        int w = image.width();
        int h = image.height();
        //model->facet_vrt
        for (int j = 0; j < 3; j++){
            vec3 v0 = model->vert(i, j);
            vec3 v1 = model->vert(i, (j + 1) % 3 );
            //std::cout<<v0<<" "<<v1<<std::endl;
            int x0 = (v0.x + 1)*w/2;
            int x1 = (v1.x + 1)*h/2;
            int y0 = (v0.y + 1)*w/2;
            int y1 = (v1.y + 1)*h/2;
            linewu(x0,y0,x1,y1, image, white);
        }
    }
}

void drawModelFaceTri(Model*& model, TGAImage& image){
    for(int i = 0; i < model->nfaces(); i++){
        int w = image.width();
        int h = image.height();
        //model->facet_vrt

        vec2 v[3];
        for (int j = 0; j < 3; j++){
            vec3 v0 = model->vert(i, j);
            v[j].x = (v0.x + 1)*w/2;
            v[j].y = (v0.y + 1)*h/2;
        }
        drawTriangle(v[0],v[1],v[2],image, TGAColor(TGAColor(rand()%255, rand()%255, rand()%255, 255)));
    }
}


// vec3 varycentric(vec2* v, vec2 p){
//     vec2 v1 = v[0];
//     vec2 v2 = v[1];
    
//     vec3 u = cross()

// }

int main(int argc, char** argv) {
    int w = 512;
    int h = 512;
    TGAImage image(w, h, TGAImage::RGB);

    Model *model = new Model("/Users/wonjongbot/microgpu/libs/tinyrenderer/obj/african_head/african_head.obj");

    drawModelFaceTri(model, image);


    // vec2 t0[3] = {vec2(10, 70),   vec2(50, 160),  vec2(70, 80)}; 
    // vec2 t1[3] = {vec2(180, 50),  vec2(150, 1),   vec2(70, 180)}; 
    // vec2 t2[3] = {vec2(180, 150), vec2(120, 160), vec2(130, 180)};

    // drawTriangle(t0[0], t0[1], t0[2], image, red);
    // drawTriangle(t1[0], t1[1], t1[2], image, green);
    // drawTriangle(t2[0], t2[1], t2[2], image, blue);


    image.write_tga_file("output.tga");
    
    return 0;
}