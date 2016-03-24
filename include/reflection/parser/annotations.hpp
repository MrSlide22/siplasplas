#ifndef SIPLASPLAS_REFLECTION_PARSER_ANNOTATIONS_HPP
#define SIPLASPLAS_REFLECTION_PARSER_ANNOTATIONS_HPP

#define SIPLASPLAS_ANNOTATION(...)

#ifndef SIPLASPLAS_REFLECTION_DISABLE_SHELLLIKE_ANNOTATION
#define $(...) SIPLASPLAS_ANNOTATION(__VA_ARGS__)
#endif // SIPLASPLAS_REFLECTION_DISABLE_SHELLLIKE_ANNOTATION

#endif // SIPLASPLAS_REFLECTION_PARSER_ANNOTATIONS_HPP
