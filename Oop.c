//
// Created by kwonryul on 23. 11. 13.
//

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "Oop.h"

/*
 *  interface Interface {
 *      void hello ();
*  }
 *
 *  class ImplSuper implements Interface {
 *      public int s
 *
 *      public ImplSuper(int s) {
 *          this.s = s;
 *      }
 *
 *      public void hello() {
 *          println("hello" + s);
 *      }
 *
 *      public void override(int x) {
 *          s = x;
 *          println("override super" + s);
 *      }
 *  }
 *
 *  class Impl extends ImplSuper {
 *      public int i;
 *
 *      public Impl(int s, int i) {
 *          super(s);
 *          this.i = i;
 *      }
 *
 *      public void override(int x) {
 *          i = x;
 *          println("override" + i);
 *      }
 *  }
 *
 *  class Impl2 implements Interface {
 *      public void hello() {
 *          println("22222");
 *      }
 *  }
 *
 *  OverrideExecutor(ImplSuper is):
 *          int x =  is.s + 1;
 *          impl.override();
 *
 *  InterfaceExecutor(Interface i):
 *          i.hello();
 *
 *  main :
 *          ImplSuper is = new ImplSuper(5);
 *          Impl i = new Impl(100, 7);
 *          Impl2 i2 = new Impl2();
 *          OverrideExecutor(is);
 *          OverrideExecutor(i);
 *          interfaceExecutor(is);
 *          interfaceExecutor(i);
 *          interfaceExecutor(i2);
 */

struct interface {
    void (*hello)(void *p);
    char *hello_name;
};

struct interface_print {
    void (*print)(void *p);
    char *print_name;
};

struct impl_super {
    struct impl_super_methods {
        struct interface interface;
        void (*override)(void *p, int x);
        char *override_name;
    } *methods;
    void (*end_of_methods)();
    struct impl_super_fields {
        int s;
    } *fields;
};

void impl_super_hello(void *p) {
    struct impl_super *is = (struct impl_super*)p;
    printf("hello %d\n", is->fields->s);
}

void impl_super_override(void *p, int x) {
    struct impl_super *is = (struct impl_super *)p;
    is->fields->s = x;
    printf("override super %d\n", is->fields->s);
}

struct impl_super_methods impl_super_methods = {
        {
            impl_super_hello,
            "hello"
        },
        impl_super_override,
        "override"
};

void impl_super_constructor(struct impl_super_fields *is_fields, int s) {
    is_fields->s = s;
}

void impl_super_init(struct impl_super* is, int s) {
    is->methods = &impl_super_methods;
    is->end_of_methods = NULL;
    is->fields = (struct impl_super_fields*)malloc(sizeof (struct impl_super_fields));
    impl_super_constructor(is->fields, s);
}

struct impl {
    struct impl_methods {
        struct impl_super_methods impl_super_methods;
        struct interface_print interface_print;
    } *methods;
    void (*end_of_methods)();
    struct impl_fields {
        struct impl_super_fields impl_super_fields;
        int y;
    } *fields;
};

void impl_override(void *p, int x) {
    struct impl *i = (struct impl*)p;
    i->fields->y = x;
    printf("override %d\n", i->fields->y);
}

void impl_print(void *p) {
    struct impl *is = (struct impl*)p;
    printf("impl_print %d\t%d\n", is->fields->impl_super_fields.s, is->fields->y);
}

struct impl_methods impl_methods = {
        {
                {
                        impl_super_hello,
                        "hello"
                },
                impl_override,
                "override"
        },
        {
            impl_print,
            "print"
        }
};

void impl_constructor(struct impl_fields *i_fields, int s, int y) {
    impl_super_constructor(i_fields, s);
    i_fields->y = y;
}

void impl_init(struct impl *i, int s, int y) {
    i->methods = &impl_methods;
    i->end_of_methods = NULL;
    i->fields = (struct impl_fields*)malloc(sizeof (struct impl_fields));
    impl_constructor(i->fields, s, y);
}

struct impl2 {
    struct impl2_methods {
        struct interface interface;
    } *methods;
    void (*end_of_methods)();
};

void impl2_hello(void *p) {
    printf("22222\n");
}

struct impl2_methods impl2_methods = {
        {
            impl2_hello,
            "hello"
        }
};

void impl2_init(struct impl2 *i2) {
    i2->methods = &impl2_methods;
    i2->end_of_methods = NULL;
}

void (*find_method(void *p, char *method))() {
    void (**function_ptr)() = *(void (***)())p;
    while (*function_ptr) {
        char **name = (char **)(function_ptr + 1);
        if (!strcmp(*name, method)) {
            return *function_ptr;
        }
        function_ptr = (void (**)())(name + 1);
    }

    return NULL;
}

void override_executor(void *p) {
    struct impl_super *is = (struct impl_super *)p;
    int x = is->fields->s + 1;
    is->methods->override(is, x);
}

void interface_executor(void *p) {
    void (*fcn)(void *) = (void (*)(void *)) find_method(p, "hello");
    if (fcn) {
        fcn(p);
    }
}

void oop() {
    struct impl_super is;
    struct impl i;
    struct impl2 i2;
    impl_super_init(&is, 100);
    impl_init(&i, 40, 5);
    impl2_init(&i2);
    void (*fcn)(void *) = (void (*)(void *))find_method(&i, "print");
    if (fcn) {
        fcn(&i);
    }
    override_executor(&is);
    override_executor(&i);
    interface_executor(&is);
    interface_executor(&i);
    interface_executor(&i2);
    if (fcn) {
        fcn(&i);
    }
}