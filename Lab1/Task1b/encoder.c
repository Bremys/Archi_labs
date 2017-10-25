

int main(int argc, char** argv){
    int i, key=0;
    char currentC;
    for(i=1; i<argc; ++i){
        if(argv[i][0]=='+' || argv[i][0] == '-'){
            key=argv[i][1]-'0';
            if(argv[i][0]=='-')
                key = key*(-1);
        }
    }
    currentC=fgetc(stdin);

            if(currentC>='a' && currentC<='z'){
                currentC+=key;
            }
        printf("%c", currentC);
        currentC=fgetc(stdin);
    }
    return 0;
}