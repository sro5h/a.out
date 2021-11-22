#include "util.h"

#include <chipmunk/chipmunk.h>
#include <stdio.h>

// TODO: Print to buffer
void aout_print_bytes(
                uint8_t* bytes, size_t size) {
        assert(bytes);

        printf("0x");
        for (size_t i = 0; i < size; ++i) {
                printf("%02x", bytes[i]);
        }
}

static
void shape_free_wrap(
                cpSpace* space,
                cpShape* shape,
                void* unused) {
        (void) unused;
        cpSpaceRemoveShape(space, shape);
        cpShapeFree(shape);
}

static
void constraint_free_wrap(
                cpSpace* space,
                cpConstraint* constraint,
                void* unused) {
        (void) unused;
        cpSpaceRemoveConstraint(space, constraint);
        cpConstraintFree(constraint);
}

static
void body_free_wrap(
                cpSpace* space,
                cpBody* body,
                void* unused) {
        (void) unused;
        cpSpaceRemoveBody(space, body);
        cpBodyFree(body);
}

static
void post_shape_free(
                cpShape* shape,
                cpSpace* space) {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc) shape_free_wrap, shape, NULL);
}

static
void post_constraint_free(
                cpConstraint* constraint,
                cpSpace* space) {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc) constraint_free_wrap, constraint, NULL);
}

static
void post_body_free(
                cpBody* body,
                cpSpace* space) {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc) body_free_wrap, body, NULL);
}

void aout_space_free_children(
                cpSpace* space) {
        // Must remove these BEFORE freeing the body or you will access dangling pointers.
        cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc) post_shape_free, space);
        cpSpaceEachConstraint(space, (cpSpaceConstraintIteratorFunc) post_constraint_free, space);

        cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc) post_body_free, space);
}
