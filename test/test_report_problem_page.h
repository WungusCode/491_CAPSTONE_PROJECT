#include <stdio.h>
#include <string.h>
#include <stdlib.h>


   // This helps us test the mailto command building logic
    //used inside report_problem_page.c without needing the
    //full GTK window.

   //This function simulates the encoding step where we
    //replace newlines (\n) with %0A for the mailto URL.

char *encode_message(const char *msg) {
    if (!msg) return NULL;

    // allocate enough space
    char *encoded = malloc(strlen(msg) * 3 + 1);
    int j = 0;

    for (int i = 0; msg[i] != '\0'; i++) {
        if (msg[i] == '\n') {
            encoded[j++] = '%';

            encoded[j++] = '0';

            encoded[j++] = 'A';
        } else {
            encoded[j++] = msg[i];
        }
    }
    encoded[j] = '\0';
    return encoded;
}


    //This helper builds the full mail command like the GUI does.

char *build_mail_command(const char *name, const char *email, const char *msg) {

    char *encoded = encode_message(msg);

    char *cmd = malloc(5000);
    snprintf(cmd, 5000,

        "open \"mailto:%s?subject=Report%%20from%%20%s&body=Name:%%20%s%%0AEmail:%%20%s%%0A%%0AMessage:%%0A%s\"",
        "RafaelMercadoEspinoza@gmail.com",
        name,
        name,
        email,

        encoded
    );

    free(encoded);
    return cmd;
}


   //  test function helper (just prints pass/fail).

void run_test(const char *title, int condition) {
    printf("[%s] %s\n", condition ? "PASS" : "FAIL", title);
}

int main() {

    // --- Test 1: Basic encoding test ---
    char *t1 = encode_message("Hello\nWorld");
    run_test("Encode newline as %0A", strcmp(t1, "Hello%0AWorld") == 0);
    free(t1);




    //  test mail
    char *cmd = build_mail_command("Rafael", "test@example.com", "This is a test\nMessage");
    int contains_subject = strstr(cmd, "subject=Report%20from%20Rafael") != NULL;

    int contains_email   = strstr(cmd, "Email:%20test@example.com") != NULL;

    int contains_encoded = strstr(cmd, "Message:%0AThis is a test%0AMessage") != NULL;

    run_test("Mail command contains subject", contains_subject);

    run_test("Mail command contains email", contains_email);
    run_test("Mail command contains encoded message", contains_encoded);


    printf("\nFull command generated:\n%s\n", cmd);

    free(cmd);

    return 0;
}