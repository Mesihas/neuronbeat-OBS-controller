// https://github.com/bramoosterhuis/esp-idf-obs-client

void authorization(uint8_t* message, bool isUpdateType, bool autho){
  if(autho){
    DeserializationError err = deserializeJson(docAuth, message);
      // Parse succeeded?
    if (err) {
      Serial.print(F("deserializeJson() returned "));
      Serial.println(err.c_str());
      return;
    }
    // Capturar error x si viene vacio PANIC KERNEL GURU ////////

    JsonObject d = docAuth["d"];
    const char* d_authentication_challenge = d["authentication"]["challenge"];
    const char* d_authentication_salt = d["authentication"]["salt"];
    const char* salt = d["authentication"]["salt"];
    const char* challenge = d["authentication"]["challenge"];
    const char* password = config.websockets_password;

    mbedtls_sha256_context *sha256_ctx = (mbedtls_sha256_context *)malloc(sizeof(mbedtls_sha256_context));
    mbedtls_sha256_free(sha256_ctx);
    unsigned char sha256[SHA256_HASH_LEN];
    size_t hashb64_len = 0;
    memset(sha256, 0, sizeof(sha256));
    mbedtls_sha256_init(sha256_ctx);
    mbedtls_sha256_starts_ret(sha256_ctx, false);
    mbedtls_sha256_update_ret(sha256_ctx, (unsigned char *)password, strlen(password));
    mbedtls_sha256_update_ret(sha256_ctx, (unsigned char *)salt, strlen(salt));
    mbedtls_sha256_finish_ret(sha256_ctx, sha256);
    mbedtls_sha256_free(sha256_ctx);

    unsigned char secret[128];
    memset(secret, 0, sizeof(secret));
    mbedtls_base64_encode(secret, sizeof(secret), &hashb64_len, sha256, sizeof(sha256));
    memset(sha256, 0, sizeof(sha256));
    mbedtls_sha256_init(sha256_ctx);
    mbedtls_sha256_starts_ret(sha256_ctx, false);
    mbedtls_sha256_update_ret(sha256_ctx, secret, hashb64_len);
    mbedtls_sha256_update_ret(sha256_ctx, (unsigned char *)challenge, strlen(challenge));
    mbedtls_sha256_finish_ret(sha256_ctx, sha256);
    mbedtls_sha256_free(sha256_ctx);

    unsigned char auth[128];
    memset(auth, 0, sizeof(auth));
    mbedtls_base64_encode(auth, sizeof(auth), &hashb64_len, sha256, sizeof(sha256));
    // Serial.println("Secret code:");
    // Serial.println((char*)auth);
    char buffer[100];

    StaticJsonDocument<192> docAuth;
    docAuth["op"] = 1;
    JsonObject dd = docAuth.createNestedObject("d");
    dd["rpcVersion"] = 1;
    dd["authentication"] = auth;
  //  dd["eventSubscriptions"] = ((1 << 2) | (1 << 7) | (1 << 11));
    char output[192];
    serializeJson(docAuth, output);
    Serial.print("output : ");
    Serial.println(output);
    webSocket.sendTXT(output); 
  }
}