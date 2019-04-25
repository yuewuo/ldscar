#include "ldscar.h"

static void run_test_thread();
static void run_test_uart1();
static void run_test_lagacy_uart1();

void run_test_case(int test_case) {
    switch(test_case) {
        case TEST_THREAD:
            run_test_thread();
            break;
        case TEST_UART1:
            run_test_uart1();
            break;
        case TEST_LEGACY_UART1:
            run_test_lagacy_uart1();
            break;
        default:
            printf("unknown test case number %d, exit\n", test_case);
            exit(-1);
    }
    printf("test finished\n");
}

static void run_test_lagacy_uart1() {
    printf("hello world\n");
    int fd;
    fd = open( "/dev/ttyS1", O_RDWR);
    if (-1 == fd) { perror("open failed"); }
    printf("open success\n");
    struct  termios Opt;
    tcgetattr(fd, &Opt);  // get current status
    cfsetispeed(&Opt, B115200);
    cfsetospeed(&Opt, B115200);
    // use RAW
    Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input*/
    Opt.c_oflag &= ~OPOST;   /*Output*/
    tcsetattr(fd, TCSANOW, &Opt);  // set immediately
    printf("init success\n");
    char buffer[32];
    strcpy(buffer, "hello");
    int length = 5;
    int nbyte;
    nbyte = write(fd, buffer, length);
    assert(nbyte == length);
    printf("send success\n");
    usleep(100000);
    nbyte = read(fd, buffer, 32);
    printf("receive %d bytes:\n", nbyte);
    buffer[nbyte] = '\0';
    printf("%s\n", buffer);
    close(fd);
}

static void* test_thread_child(void* data) {
    // pthread_cond_t cond;
    // pthread_mutex_t mutex;
    // struct timespec outtime;
    // pthread_cond_init(&cond);
    // pthread_mutex_init(&mutex);
    // pthread_mutex_lock(&mutex);
    // clock_gettime(CLOCK_REALTIME, &outtime);
    // outtime.tv_sec += 1;
    // pthread_cond_timedwait(&cond, &mutex, outtime);
    // pthread_cond_destroy(&cond);
    // pthread_mutex_destroy(&mutex);

    if (*(int*)data) usleep(500000);
    for (int i=0; i<3; ++i) {
        sleep(1);
        printf("thread %d\n", *(int*)data);
    }
}

static void run_test_thread() {
    pthread_t t1, t2;
    int data1 = 0, data2 = 1;
    pthread_create(&t1, NULL, test_thread_child, &data1);
    pthread_create(&t2, NULL, test_thread_child, &data2);
    printf("wait for joining...\n");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

static void* test_uart1_wait(void* data) {
    void** args = (void**)data;
    int& exit_uart1 = *(int*)args[0];
    pthread_mutex_t& mutex = *(pthread_mutex_t*)args[1];
    Serial& tty = *(Serial*)args[2];
    while (!exit_uart1) {
        string buf = tty.read(32);
        pthread_mutex_lock(&mutex);
        if (buf.length()) printf("RECV: %s\n", buf.c_str());
        pthread_mutex_unlock(&mutex);
    }
}
static void run_test_uart1() {
    const char* device = "/dev/ttyS1";
    printf("opening %s...\n", device);
    Serial tty(device, 115200, serial::Timeout::simpleTimeout(1000));
    if (tty.isOpen()) printf("tty opened\n");
    else { printf("open tty failed, exit\n"); exit(-1); }

    int flag;
    if ((flag = fcntl(STDIN_FILENO, F_GETFL, 0)) < 0 || fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK) < 0) {
        printf("setup stdin as non-blocking failed, exit\n"); exit(-1);
    }

    int exit_uart1 = 0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_t t1;
    printf("press ENTER to stop\n");
    void* args[3] = { &exit_uart1, &mutex, &tty };
    pthread_create(&t1, NULL, test_uart1_wait, &args);
    char buf[2]; while (read(STDIN_FILENO, buf, 1) <= 0) {
        sleep(1);
        tty.write(string("hello\n"));
        pthread_mutex_lock(&mutex);
        printf("SENT: hello\n");
        pthread_mutex_unlock(&mutex);
    }

    printf("waiting for thread to join\n");
    exit_uart1 = 1;
    pthread_join(t1, NULL);
    pthread_mutex_destroy(&mutex);

    tty.close();
    printf("tty closed\n");
}
