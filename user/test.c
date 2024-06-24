int cd = channel_create();
if (cd < 0) {
printf("Failed to create channel\n");
exit(1);
}
if (fork() == 0) {
if (channel_put(cd, 42) < 0) {
printf("Failed to put data in channel\n");
exit(1);
}
channel_put(cd, 43); // Sleeps until cleared
// Handle error
channel_destroy(cd);
// Handle error
} else {
int data;
if (channel_take(cd, &data) < 0) { // 42
printf("Failed to take data from channel\n");