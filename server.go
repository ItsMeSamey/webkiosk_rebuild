package main

/*
 #include "caller.h"
*/
import (
	"C"
	"github.com/gin-gonic/gin"
)

func static_server() {
	router := gin.Default()
	router.MaxMultipartMemory = 64 << 20 // 64 MiB (more than enough)
	router.Static("/app", "./dist")
	router.Static("/assets", "./dist/assets/")
	router.Run("127.0.0.1:3000")
}

func main() {
	curlHandle := C.get_auth_curl()
	static_server()
}

