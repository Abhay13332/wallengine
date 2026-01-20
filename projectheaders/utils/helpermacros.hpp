#ifndef HELP_MACROS_HPP
#define macro(HELP_MACROS_HPP)

#define assert_no_err(val) if(val==-1) err(to_string(val))
#define check_glsl_error() \
{GLenum err; \
while((err=glGetError()) !=GL_NO_ERROR)\
  std::cout << "opengl error:" << err << "at file:"<<__FILE__ << "at line:" << __LINE__ ;\
}
#endif 