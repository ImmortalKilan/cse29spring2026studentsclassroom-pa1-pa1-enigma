#include <stdio.h>
#include <stdlib.h>
// Do not include other standard library headers

#define ALPHABET_SIZE 26
#define NUM_ROTORS 8

/* Array of rotors */
const char* enigma_rotors[NUM_ROTORS+1] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ", //0
    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", //1
    "AJDKSIRUXBLHWTMCQGZNPYFVOE", //2
    "BDFHJLCPRTXVZNYEIWGAKMUSQO", //3
    "ESOVPZJAYQUIRHXLNFTGKDCMWB", //4
    "UKLBEPXWJVFZIYGAHCMTONDRQS", //5
    "JPGVOUMFYQBENHZRDKASXLICTW", //6
    "NZJHGRCXMYSWBOUFAIVLPEKQDT", //7
    "FKQHTLXOCBJSPDZRAMEWNIUYGV"  //8
};

/*
 * Convert a space-separated string of rotor indices into
 * an integer array of rotor indices
 *
 * @param rotor_ind_str   Space-separated string of rotor indices
 * @param num_rotors      Number of rotors provided in the string
 * @return                Integer array of rotor indices
 */
int* parse_rotor_indices(char* rotor_ind_str, int num_rotors) {
    int *arr = malloc(num_rotors * sizeof(int));
    if (arr == NULL){
        fprintf(stderr, "Memory allocation failed!\n");
        exit(1);
    }
    int arr_index = 0;
    int str_index = 0;
    int current_num = 0;
    int is_reading = 0; // boolean value
    while (rotor_ind_str[str_index] != '\0' && arr_index < num_rotors){
        char c = rotor_ind_str[str_index];
        if (c >= '0' && c <= '9'){
            current_num = current_num * 10 + c -'0';
            is_reading = 1;
        }
        else if(c == ' '){
            if (is_reading == 1){
                arr[arr_index] = current_num;
                arr_index++;
                current_num = 0;
                is_reading = 0;
            }
        }
        str_index++;
    }
    if (is_reading == 1 && arr_index < num_rotors){
        arr[arr_index] = current_num;
    }
    return arr;
    
}


void test_parse_rotor_indices(){
    int expected_result[3] = {1,2,3};
    int *actual_result = parse_rotor_indices("1 2 3",3);
    printf("%d\n",actual_result[0]);
    int match = 1;
    if (actual_result == NULL){
        printf("return null pointer!\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        if (actual_result[i] != expected_result[i]) {
            match = 0;
            break;
        }
    }
    if (!match){
        printf("test failed!\n");
    }
    free(actual_result);
}


/*
 * Create a 2D array of integers where each row represents a rotor.
 * Review the PA specification for examples
 *
 * @param rotor_indices   Integer array of rotor indices
 * @param num_rotors      Number of rotors to apply
 * @return                2D array where each row represents a rotor
 */
int **set_up_rotors(int* rotor_indices, int num_rotors) {
    int** index_array = malloc(num_rotors * sizeof(char*));
    if (index_array == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; i < num_rotors; i++){
        int* rotor_in_decimal = malloc(26 * sizeof(int));
        for (int j = 0; j < 26; j++){
            const char* rotor = enigma_rotors[rotor_indices[i]];
            rotor_in_decimal[j] = rotor[j] - 'A';
        }
        index_array[i] = rotor_in_decimal;
    }
    return index_array;
}

void test_set_up_rotors(){
    int rotor_indices[3] = {1,3,4};
    int** rotor_config = set_up_rotors(rotor_indices, 3);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 26; j++){
            printf("%d ", rotor_config[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < 3; i++){
        free(rotor_config[i]);
    }
    free(rotor_config);
}


/*
 * Rotate each rotor to the right by the given number of rotations
 *
 * @param rotor_config   2D array of rotors
 * @param num_rotors     Number of rotors provided
 * @param rotations      Number of rotations
 */
void rotate_rotors(int** rotor_config, int num_rotors, int rotations) {
    for (int row = 0; row < num_rotors; row++){
        int* temp_arr = rotor_config[row];
        for (int i = 0; i < rotations; i++){
            int temp = temp_arr[25];
            for (int j = 25; j > 0; j--){
                temp_arr[j] = temp_arr[j-1];
            }
            temp_arr[0] = temp;
        }
    }
}

void test_rotate_rotors(){
    int rotor_indices[3] = {1,3,4};
    int** rotor_config = set_up_rotors(rotor_indices, 3);
    rotate_rotors(rotor_config, 3, 29);
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 26; j++){
            printf("%d ", rotor_config[i][j]);
        }
        printf("\n");
    }
    for (int i = 0; i < 3; i++){
        free(rotor_config[i]);
    }
    free(rotor_config);
}

int my_strlen(char* str){
    int length = 0;
    while (str[length] != '\0'){
        length++;
    }
    return length;
}
/*
 * Encrypt the given message
 *
 * @param message        Message to encrypt
 * @param rotor_config   2D array of rotors
 * @param num_rotors     Number of rotors provided
 * @return               Encrypted message
 */
char* encrypt(char *message, int** rotor_config, int num_rotors) {
    char* encrypted_message = malloc((my_strlen(message) + 1) * sizeof(char));
    if (encrypted_message == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; message[i] != '\0'; i++){
        char c = message[i];
        if (c >= 'A' && c <= 'Z'){
            int index = c - 'A';
            for (int j = 0; j < num_rotors; j++){
                index = rotor_config[j][index];
            }
            encrypted_message[i] = index + 'A';
        }
        else{
            encrypted_message[i] = c;
        }
    }
    encrypted_message[my_strlen(message)] = '\0';
    return encrypted_message;
}

void test_encrypt(){
    int rotor_indices[3] = {1,3,4};
    int** rotor_config = set_up_rotors(rotor_indices, 3);
    char* encrypted_message = encrypt("HELLO WORLD", rotor_config, 3);
    printf("%s\n", encrypted_message);
    free(encrypted_message);
    for (int i = 0; i < 3; i++){
        free(rotor_config[i]);
    }
    free(rotor_config);
}

/*
 * Decrypt the given message
 *
 * @param message        Message to decrypt
 * @param rotor_config   2D array of rotors
 * @param num_rotors     Number of rotors provided
 * @return               Decrypted message
 */
char* decrypt(char *message, int** rotor_config, int num_rotors) {
    char* decrypted_message = malloc((my_strlen(message) + 1) * sizeof(char));
    if (decrypted_message == NULL){
        printf("Memory allocation failed!\n");
        exit(1);
    }
    for (int i = 0; message[i] != '\0'; i++){
        char c = message[i];
        if (c >= 'A' && c <= 'Z'){
            int index = c - 'A';
            for (int j = num_rotors - 1; j >= 0; j--){
                for (int k = 0; k < ALPHABET_SIZE; k++){
                    if (rotor_config[j][k] == index){
                        index = k;
                        break;
                    }
                }
            }
            decrypted_message[i] = index + 'A';
        }
        else{
            decrypted_message[i] = c;
        }
    }
    decrypted_message[my_strlen(message)] = '\0';
    return decrypted_message;
}

void test_decrypt(){
    int rotor_indices[3] = {1,3,4};
    int** rotor_config = set_up_rotors(rotor_indices, 3);
    char* decrypted_message = decrypt("MFNCZ YGJMG", rotor_config, 3);
    printf("%s\n", decrypted_message);
    free(decrypted_message);
    for (int i = 0; i < 3; i++){
        free(rotor_config[i]);
    }
    free(rotor_config);
}
/*
 * Format of command line input:
 * ./enigma e "JAVA" 3 "1 2 4" 0
 * 
 *    e    - mode (e for encrypt, d for decrypt)
 * "JAVA"  - message
 *    3    - number of rotors to use between 1 and 9, inclusive
 * "1 2 4" - sequence of indices of rotors to apply, separated by spaces
 *    0    - number of rotations of the rotors
 */
int main(int argc, char* argv[]) {
    char mode = argv[1][0];
    char* message = argv[2];
    int num_rotors = atoi(argv[3]);
    char* rotor_indices_str = argv[4];
    int rotations = atoi(argv[5]);
    int* rotor_indices = parse_rotor_indices(rotor_indices_str, num_rotors);
    int** rotor_config = set_up_rotors(rotor_indices, num_rotors);
    if (mode == 'e'){
        if (rotations > 0){
            rotate_rotors(rotor_config, num_rotors, rotations);
        }
        char* encrypted_message = encrypt(message, rotor_config, num_rotors);
        printf("%s\n", encrypted_message);
        free(encrypted_message);
    }
    else if (mode == 'd'){
        if (rotations > 0){
            rotate_rotors(rotor_config, num_rotors, rotations);
        }
        char* decrypted_message = decrypt(message, rotor_config, num_rotors);
        printf("%s\n", decrypted_message);
        free(decrypted_message);
    }
    else{
        printf("Invalid mode! Use 'e' for encrypt and 'd' for decrypt.\n");
    }
    return 0;
}