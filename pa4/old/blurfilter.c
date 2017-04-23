
//array

int [] blur(pixel chunk [], int radius, int x, int y, int imageX, int imageY){
    int minX;
    int minY;
    int maxX;
    int maxY;
    minX = x - radius;
    minY = y - radius;
    maxX = x + radius;
    maxY = y + radius
    if (minX < 0){
        minX = 0;       
    }else if(minY < 0 ){
        minY = 0;
    }else if(maxX > imageX){
        maxX = iamgeX;
    }else if(maxY > imageY){
        maxY = imageY;
    }

    int sumR;
    int sumG;
    int sumB;
    int numOfPixels = 0;

    for (int i = minX; i < maxX; i++){
        for(int j = minY; j < maxY; j++ ){
            sumR = chunk[i][j]->r + sumR;
            sumG = chunk[i][j]->g + sumG;
            sumB = chunk[i][j]->b + sumB;
            numOfPixels = numOfPixels + 1;
        }
    }
    int retR = sumR/numOfPixels;
    int retG = sumG/numOfPixels;
    int retB = sumB/numOfPixels
}