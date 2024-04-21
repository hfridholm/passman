#ifndef P_WINDOW_H
#define P_WINDOW_H

extern void window_title_center_print(window_t* window, const char* title);

extern void window_free(window_t* window);

extern void window_clean(window_t* window);

extern window_t* window_create(int x, int y, int w, int h);

extern void window_resize(window_t* window, int x, int y, int w, int h);

#endif
