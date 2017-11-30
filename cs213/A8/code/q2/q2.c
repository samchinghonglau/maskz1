int p1;
int p2;
int p3;
int val;

int q2(int a, int b, int c){
    int temp;
    switch(a){
        case 10:
            temp = b + c;
            break;
        case 12:
            temp = b - c;
            break;
        case 14:
            if (b > c){
                c = 1;
                temp = c;
            } else {
                c = 0;
                temp = c;
            }
            break;
        case 16:
            if (c > b){
                c = 1;
                temp = c;
            } else {
                c = 0;
                temp = c;
            }
            break;
        case 18:
            if (b == c){
                c = 1;
                temp = c;
            } else {
                c = 0;
                temp = c;
            }
            break;
        default:
            temp = 0;
            break;        
    }
    return temp;
}

int main(void){
    val = q2(p1, p2, p3);
}