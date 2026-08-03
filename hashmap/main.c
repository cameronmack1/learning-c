#include "hashmap.h"
#include <stdio.h>

int main() {
    //init hashmap with 4 buckets
    Hashmap* map = init(4);
    //add "banana": 12 to map
    int value1 = 12;
    if(add(map, "banana", &value1))
        printf("\"banana\": %d added\n", value1);
    //add "wow: 4" to map
    int value2 = 4;
    if(add(map, "wow", &value2))
        printf("\"wow\": %d added\n", value2);
    //get and print out "banana" key
    int* returnVal = NULL;
    //only print if get succeeded
    if(get(map, "banana", (void**)&returnVal)) {
        char str[12];
        snprintf(str, 12, "%d", *returnVal);
        printf("banana is %s\n", str);
    }
    //replace "banana" with 99999
    int value3 = 99999;
    if(add(map, "banana", &value3))
        printf("\"banana\": %d added\n", value3);

    //print banana again
    if(get(map, "banana", (void**)&returnVal)) {
        char str[12];
        snprintf(str, 12, "%d", *returnVal);
        printf("banana is %s\n", str);
    }

    printf("map has %zu elements\n", map->count);
    //remove banana
    rem(map, "banana");
    printf("removed banana\n");
    printf("map has %zu elements\n", map->count);

    //attempt to get "banana"
    if(get(map, "banana", (void**)&returnVal)) {
        //print value if it exists
        char str[12];
        snprintf(str, 12, "%d", *returnVal);
        printf("banana is %s\n", str);
    } else {
        //print if it doesnt exist
        printf("\"banana\" is not an existing key\n");
    }
}