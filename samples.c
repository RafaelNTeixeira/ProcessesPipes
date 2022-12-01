int main(int argc, char* argv[]){
    if (argc != 4){
        printf("%s","usage: samples file numberfrags maxfragsize");
        exit(0);
    }
    FILE* fp;
    fp = fopen(argv[1],"r");
    int lower = 0;
    int n = atoi(argv[2]);
    int m = atoi(argv[3]);
    fseek(fp, 0, SEEK_END);
    int upper = ftell(fp);
    srand(time(0));
    for (int i = 0; i < n; i++) {
        printf("%c", '>');
        int random = ((rand() % (upper - lower + 1)) + lower) - m;
        fseek(fp, random, SEEK_SET);
        char* ptr = (char*) malloc(m * sizeof(char));
        fread(ptr, 1, m, fp);
        printf("%s", ptr);
        printf("%c\n", '<');
    }
    fclose(fp);
}