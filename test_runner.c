/* TODO: put license here. */

#include <dlfcn.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

ssize_t GetSelfDirectory(char* buffer, size_t max_length)
{
  char* end;
  if (readlink("/proc/self/exe", buffer, max_length) == -1) {
    perror("readlink");
    abort();
  }
  end = strrchr(buffer, '/');
  if (end == NULL) {
    abort();
  }
  ++end;
  *end = '\0';
  return (end - buffer);
}

int main()
{
  char full_library_path[PATH_MAX];
  char library_name[] = "library.so";

  ssize_t dir_length = GetSelfDirectory(full_library_path, PATH_MAX);
  strncpy(full_library_path + dir_length, library_name, sizeof(library_name));
  void* handle = dlopen(full_library_path, RTLD_LAZY);
  if (!handle) {
    fprintf(stderr, "error: %s\n", dlerror());
    abort();
  }

  dlerror();
  typedef void (*RunBenchmarkFunction)();
  RunBenchmarkFunction run_benchmark =
    (RunBenchmarkFunction) dlsym(handle, "RunBenchmark");
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    fprintf(stderr, "error: %s\n", dlsym_error);
    dlclose(handle);
    abort();
  }

  run_benchmark();

  dlclose(handle);
  return 0;
}
