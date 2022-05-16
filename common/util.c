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
void space_shape_free_wrap(
                cpSpace* space,
                void* key,
                void* unused) {
        (void) unused;
        cpShape* shape = key;
        cpSpaceRemoveShape(space, shape);
        cpShapeFree(shape);
}

static
void space_constraint_free_wrap(
                cpSpace* space,
                void* key,
                void* unused) {
        (void) unused;
        cpConstraint* constraint = key;
        cpSpaceRemoveConstraint(space, constraint);
        cpConstraintFree(constraint);
}

static
void space_body_free_wrap(
                cpSpace* space,
                void* key,
                void* unused) {
        (void) unused;
        cpBody* body = key;
        cpSpaceRemoveBody(space, body);
        cpBodyFree(body);
}

static
void space_post_shape_free(
                cpShape* shape,
                void* context) {
        cpSpace* space = context;
        cpSpaceAddPostStepCallback(space, space_shape_free_wrap, shape, NULL);
}

static
void space_post_constraint_free(
                cpConstraint* constraint,
                void* context) {
        cpSpace* space = context;
        cpSpaceAddPostStepCallback(space, space_constraint_free_wrap, constraint, NULL);
}

static
void space_post_body_free(
                cpBody* body,
                void* context) {
        cpSpace* space = context;
        cpSpaceAddPostStepCallback(space, space_body_free_wrap, body, NULL);
}

void aout_space_free_children_post_step(
                cpSpace* space) {
        cpSpaceEachShape(space, space_post_shape_free, space);
        cpSpaceEachConstraint(space, space_post_constraint_free, space);
        // TODO: Remove arbiters aswell
        // Must remove the above BEFORE freeing the body
        cpSpaceEachBody(space, space_post_body_free, space);
}

void aout_space_del(
                cpSpace** out_space) {
        assert(out_space);

        if (*out_space) {
                aout_space_free_children_post_step(*out_space);
                cpSpaceFree(*out_space);
                *out_space = NULL;
        }
}

static
void body_shape_free_wrap(
                cpBody* body,
                cpShape* shape,
                void* unused) {
        (void) unused;
        cpSpaceRemoveShape(cpBodyGetSpace(body), shape);
        cpShapeFree(shape);
}

static
void body_constraint_free_wrap(
                cpBody* body,
                cpConstraint* constraint,
                void* unused) {
        (void) unused;
        cpSpaceRemoveConstraint(cpBodyGetSpace(body), constraint);
        cpConstraintFree(constraint);
}

void aout_body_free_children(
                cpBody* body) {
        cpBodyEachShape(body, body_shape_free_wrap, NULL);
        cpBodyEachConstraint(body, body_constraint_free_wrap, NULL);
        // TODO: Free arbiters aswell
}

void aout_body_free(
                cpBody* body) {
        if (body) {
                aout_body_free_children(body);
                cpSpaceRemoveBody(cpBodyGetSpace(body), body);
                cpBodyFree(body);
        }
}
