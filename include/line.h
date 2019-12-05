#ifndef _LINE_H_
#define _LINE_H_
void* line_create();

void line_push(void* ptr,float x,float y,float z);

void line_dispose(void* ptr);

void line_render(void* ptr);

#endif