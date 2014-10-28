

void unmapearPagina(void* direccionVirtual, void* directorioDePaginas,void* pagina);

void mapearPagina(void *direccionReal, void* direccionVirtual, void* directorioDePaginas,void* pagina);

void crearPageDirectory(void * direccion);

void crearPageTable(void * direccion);