int foo_done = 0; //Similar to flags in pseudocode
int bar_done = 0; //Similar to flags in pseudocode

typedef struct {
    int n;
    pthread_mutex_t mutex;
    pthread_cond_t cond1;
    pthread_cond_t cond2;
} FooBar;

FooBar* fooBarCreate(int n) {
    FooBar* obj = (FooBar*) malloc(sizeof(FooBar));
    obj->n = n;
    pthread_mutex_init(&(obj->mutex), NULL); //Initialize
    pthread_cond_init(&(obj->cond1), NULL); //Initialize cond1
    pthread_cond_init(&(obj->cond2), NULL); //Initialize cond2
    return obj;
}

void foo(FooBar* obj) {
    
    for (int i = 0; i < obj->n; i++) {
        // Lock
        pthread_mutex_lock(&(obj->mutex));
        while(foo_done == 1){ 
            pthread_cond_wait(&obj->cond1, &(obj->mutex)); //wait function will release lock and go to sleep. When it returns it is holding the lock again
        }
        // printFoo() outputs "foo". Do not change or remove this line.
        printFoo();
        foo_done=1;
        bar_done=0;
        pthread_cond_signal(&obj->cond2); //wake up cond2 who is waiting
        pthread_mutex_unlock(&(obj->mutex));
    }
}

void bar(FooBar* obj) {
    
    for (int i = 0; i < obj->n; i++) {
        pthread_mutex_lock(&(obj->mutex));
        while(foo_done==0 || bar_done==1 ){ //
            pthread_cond_wait(&obj->cond2, &(obj->mutex));
        }
        // printBar() outputs "bar". Do not change or remove this line.
        printBar();
        foo_done=0;
        bar_done=1; 
        pthread_cond_signal(&obj->cond1); //wake cond1 up who is waiting
        pthread_mutex_unlock(&(obj->mutex));
    }
}

void fooBarFree(FooBar* obj) {
    pthread_mutex_destroy(&(obj->mutex));
    pthread_cond_destroy(&obj->cond1);
    pthread_cond_destroy(&obj->cond2);
    free(obj);
    
}
