/*  Pawn compiler  - maintenance of various lists
 *
 *  o  Name list (aliases)
 *  o  Include path list
 *  o  Macro defintions (text substitutions)
 *
 *  Copyright (c) ITB CompuPhase, 2001-2005
 *
 *  This software is provided "as-is", without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1.  The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software in
 *      a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *  2.  Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *  3.  This notice may not be removed or altered from any source distribution.
 */

#include "sc.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#if defined FORTIFY
    #include "fortify.h"
#endif

/* a "private" implementation of strdup(), so that porting
 * to other memory allocators becomes easier.
 * By Soren Hannibal.
 */
SC_FUNC char* duplicatestring(const char* sourcestring)
{
    char* result = malloc(strlen(sourcestring) + 1);
    strcpy(result, sourcestring);
    return result;
}

static stringpair* insert_stringpair(stringpair* root, const char* first, const char* second, const int matchlength)
{
    stringpair *cur, *pred;

    assert(root != NULL);
    assert(first != NULL);
    assert(second != NULL);
    /* create a new node, and check whether all is okay */
    if ((cur = (stringpair*)malloc(sizeof(stringpair))) == NULL) {
        return NULL;
    }
    cur->first = duplicatestring(first);
    cur->second = duplicatestring(second);
    cur->matchlength = matchlength;
    if (cur->first == NULL || cur->second == NULL) {
        if (cur->first != NULL) {
            free(cur->first);
        }
        if (cur->second != NULL) {
            free(cur->second);
        }
        free(cur);
        return NULL;
    } /* if */
    /* link the node to the tree, find the position */
    for (pred = root; pred->next != NULL && strcmp(pred->next->first, first) < 0; pred = pred->next)
        /* nothing */;
    cur->next = pred->next;
    pred->next = cur;
    return cur;
}

static void delete_stringpairtable(stringpair* root)
{

    assert(root != NULL);
    stringpair* cur = root->next;
    while (cur != NULL) {
        stringpair* next = cur->next;
        assert(cur->first != NULL);
        assert(cur->second != NULL);
        free(cur->first);
        free(cur->second);
        free(cur);
        cur = next;
    } /* while */
    memset(root, 0, sizeof(stringpair));
}

static stringpair* find_stringpair(stringpair* cur, const char* first, const int matchlength)
{
    int result = 0;

    assert(matchlength > 0); /* the function cannot handle zero-length comparison */
    assert(first != NULL);
    while (cur != NULL && result <= 0) {
        result = (int)*cur->first - (int)*first;
        if (result == 0 && matchlength == cur->matchlength) {
            result = strncmp(cur->first, first, matchlength);
            if (result == 0) {
                return cur;
            }
        } /* if */
        cur = cur->next;
    } /* while */
    return NULL;
}

static int delete_stringpair(stringpair* root, stringpair* item)
{

    assert(root != NULL);
    stringpair* cur = root;
    while (cur->next != NULL) {
        if (cur->next == item) {
            cur->next = item->next; /* unlink from list */
            assert(item->first != NULL);
            assert(item->second != NULL);
            free(item->first);
            free(item->second);
            free(item);
            return TRUE;
        } /* if */
        cur = cur->next;
    } /* while */
    return FALSE;
}

/* ----- string list functions ----------------------------------- */
static stringlist* insert_string(stringlist* root, const char* string)
{
    stringlist* cur;

    assert(string != NULL);
    if ((cur = (stringlist*)malloc(sizeof(stringlist))) == NULL) {
        error(103); /* insufficient memory (fatal error) */
    }
    if ((cur->line = duplicatestring(string)) == NULL) {
        error(103); /* insufficient memory (fatal error) */
    }
    /* insert as "last" */
    assert(root != NULL);
    while (root->next != NULL) {
        root = root->next;
    }
    cur->next = root->next;
    root->next = cur;
    return cur;
}

static char* get_string(const stringlist* root, int index)
{

    assert(root != NULL);
    const stringlist* cur = root->next;
    while (cur != NULL && index-- > 0) {
        cur = cur->next;
    }
    if (cur != NULL) {
        assert(cur->line != NULL);
        return cur->line;
    } /* if */
    return NULL;
}

static int delete_string(stringlist* root, int index)
{
    stringlist* cur;

    assert(root != NULL);
    for (cur = root; cur->next != NULL && index > 0; cur = cur->next, index--)
        /* nothing */;
    if (cur->next != NULL) {
        stringlist* item = cur->next;
        cur->next = item->next; /* unlink from list */
        assert(item->line != NULL);
        free(item->line);
        free(item);
        return TRUE;
    } /* if */
    return FALSE;
}

SC_FUNC void delete_stringtable(stringlist* root)
{

    assert(root != NULL);
    stringlist* cur = root->next;
    while (cur != NULL) {
        stringlist* next = cur->next;
        assert(cur->line != NULL);
        free(cur->line);
        free(cur);
        cur = next;
    } /* while */
    memset(root, 0, sizeof(stringlist));
}

/* ----- alias table --------------------------------------------- */
static stringpair alias_tab = {NULL, NULL, NULL}; /* alias table */

SC_FUNC stringpair* insert_alias(const char* name, const char* alias)
{
    stringpair* cur;

    assert(name != NULL);
    assert(strlen(name) <= sNAMEMAX);
    assert(alias != NULL);
    assert(strlen(alias) <= sNAMEMAX);
    if ((cur = insert_stringpair(&alias_tab, name, alias, strlen(name))) == NULL) {
        error(103); /* insufficient memory (fatal error) */
    }
    return cur;
}

SC_FUNC int lookup_alias(char* target, const char* name)
{
    const stringpair* cur = find_stringpair(alias_tab.next, name, strlen(name));
    if (cur != NULL) {
        assert(strlen(cur->second) <= sNAMEMAX);
        strcpy(target, cur->second);
    } /* if */
    return cur != NULL;
}

SC_FUNC void delete_aliastable(void)
{
    delete_stringpairtable(&alias_tab);
}

/* ----- include paths list -------------------------------------- */
static stringlist includepaths = {NULL, NULL}; /* directory list for include files */

SC_FUNC stringlist* insert_path(const char* path)
{
    return insert_string(&includepaths, path);
}

SC_FUNC char* get_path(const int index)
{
    return get_string(&includepaths, index);
}

SC_FUNC void delete_pathtable(void)
{
    delete_stringtable(&includepaths);
    assert(includepaths.next == NULL);
}

/* ----- text substitution patterns ------------------------------ */
#if !defined NO_DEFINE

static stringpair substpair = {NULL, NULL, NULL}; /* list of substitution pairs */

static stringpair* substindex['z' - PUBLIC_CHAR + 1]; /* quick index to first character */
static void adjustindex(const char c)
{
    stringpair* cur;
    assert(c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_' || c == PUBLIC_CHAR);
    assert(PUBLIC_CHAR < 'A' && 'A' < '_' && '_' < 'z');

    for (cur = substpair.next; cur != NULL && cur->first[0] != c; cur = cur->next)
        /* nothing */;
    substindex[(int)c - PUBLIC_CHAR] = cur;
}

SC_FUNC stringpair* insert_subst(const char* pattern, const char* substitution, const int prefixlen)
{
    stringpair* cur;

    assert(pattern != NULL);
    assert(substitution != NULL);
    if ((cur = insert_stringpair(&substpair, pattern, substitution, prefixlen)) == NULL) {
        error(103); /* insufficient memory (fatal error) */
    }
    adjustindex(*pattern);

    if (pc_deprecate != NULL) {
        assert(cur != NULL);
        cur->flags |= flgDEPRECATED;
        if (sc_status == statWRITE) {
            if (cur->documentation != NULL) {
                free(cur->documentation);
                cur->documentation = NULL;
            } /* if */
            cur->documentation = pc_deprecate;
        }
        else {
            free(pc_deprecate);
        } /* if */
        pc_deprecate = NULL;
    }
    else {
        cur->flags = 0;
        cur->documentation = NULL;
    } /* if */
    return cur;
}

SC_FUNC stringpair* find_subst(const char* name, const int length)
{
    assert(name != NULL);
    assert(length > 0);
    assert(*name >= 'A' && *name <= 'Z' || *name >= 'a' && *name <= 'z' || *name == '_' || *name == PUBLIC_CHAR);
    stringpair* item = substindex[(int)*name - PUBLIC_CHAR];
    if (item != NULL) {
        item = find_stringpair(item, name, length);
    }

    if (item && (item->flags & flgDEPRECATED) != 0) {
        static char macro[128];
        char *rem, *msg = item->documentation != NULL ? item->documentation : "";
        strncpy(macro, item->first, sizeof(macro));
        macro[sizeof(macro) - 1] = '\0';

        /* If macro contains an opening parentheses and a percent sign, then assume that
         * it takes arguments and remove them from the warning message.
         */
        if ((rem = strchr(macro, '(')) != NULL && strchr(macro, '%') > rem) {
            *rem = '\0';
        }

        error(233, macro, msg); /* deprecated (macro/constant) */
    }
    return item;
}

SC_FUNC int delete_subst(const char* name, const int length)
{
    assert(name != NULL);
    assert(length > 0);
    assert(*name >= 'A' && *name <= 'Z' || *name >= 'a' && *name <= 'z' || *name == '_' || *name == PUBLIC_CHAR);
    stringpair* item = substindex[(int)*name - PUBLIC_CHAR];
    if (item != NULL) {
        item = find_stringpair(item, name, length);
    }
    if (item == NULL) {
        return FALSE;
    }
    delete_stringpair(&substpair, item);
    adjustindex(*name);
    return TRUE;
}

SC_FUNC void delete_substtable(void)
{
    delete_stringpairtable(&substpair);
    for (int i = 0; i < sizeof substindex / sizeof substindex[0]; i++) {
        substindex[i] = NULL;
    }
}

#endif /* !defined NO_SUBST */

/* ----- input file list (explicit files)------------------------- */
static stringlist sourcefiles = {NULL, NULL};
static char first_source_file[_MAX_PATH] = {0};

SC_FUNC const char* pop_first_source_file(void)
{
    static char buffer[_MAX_PATH];
    const int written = snprintf(buffer, sizeof(buffer), "%s", first_source_file);
    if (written < 0 || written >= (int)sizeof(buffer)) {
        buffer[sizeof(buffer) - 1] = '\0';
    }
    memset(first_source_file, 0, sizeof(first_source_file));
    return buffer;
}

SC_FUNC stringlist* insert_sourcefile(const char* string)
{
    if (get_sourcefile(0) == NULL) {
        strncpy(first_source_file, string, sizeof(first_source_file) - 1);
        first_source_file[sizeof(first_source_file) - 1] = '\0';
    }
    return insert_string(&sourcefiles, string);
}

SC_FUNC char* get_sourcefile(const int index)
{
    return get_string(&sourcefiles, index);
}

SC_FUNC void delete_sourcefiletable(void)
{
    delete_stringtable(&sourcefiles);
    assert(sourcefiles.next == NULL);
}

/* ----- parsed file list (explicit + included files) ------------ */
static stringlist inputfiles = {NULL, NULL};

SC_FUNC stringlist* insert_inputfile(const char* string)
{
    if (sc_status != statFIRST) {
        return insert_string(&inputfiles, string);
    }
    return NULL;
}

SC_FUNC char* get_inputfile(const int index)
{
    return get_string(&inputfiles, index);
}

SC_FUNC void delete_inputfiletable(void)
{
    delete_stringtable(&inputfiles);
    assert(inputfiles.next == NULL);
}

/* ----- documentation tags -------------------------------------- */
#if !defined SC_LIGHT
static stringlist docstrings = {NULL, NULL};

SC_FUNC stringlist* insert_docstring(const char* string)
{
    return insert_string(&docstrings, string);
}

SC_FUNC char* get_docstring(const int index)
{
    return get_string(&docstrings, index);
}

SC_FUNC void delete_docstring(const int index)
{
    delete_string(&docstrings, index);
}

SC_FUNC void delete_docstringtable(void)
{
    delete_stringtable(&docstrings);
    assert(docstrings.next == NULL);
}
#endif /* !defined SC_LIGHT */

/* ----- autolisting --------------------------------------------- */
static stringlist autolist = {NULL, NULL};

SC_FUNC stringlist* insert_autolist(const char* string)
{
    return insert_string(&autolist, string);
}

SC_FUNC char* get_autolist(const int index)
{
    return get_string(&autolist, index);
}

SC_FUNC void delete_autolisttable(void)
{
    delete_stringtable(&autolist);
    assert(autolist.next == NULL);
}

/* ----- value pair list ----------------------------------------- */
static valuepair heaplist = {NULL, 0, 0};

SC_FUNC valuepair* push_heaplist(const long first, const long second)
{
    valuepair *cur, *last;
    if ((cur = malloc(sizeof(valuepair))) == NULL) {
        error(103); /* insufficient memory (fatal error) */
    }

    cur->first = first;
    cur->second = second;
    cur->next = NULL;

    for (last = &heaplist; last->next != NULL; last = last->next)
        /* nothing */;
    last->next = cur;
    return cur;
}

SC_FUNC int popfront_heaplist(long* first, long* second)
{
    valuepair* front = heaplist.next;
    if (front == NULL) {
        return 0;
    }

    /* copy fields */
    *first = front->first;
    *second = front->second;

    /* unlink and free */
    heaplist.next = front->next;
    free(front);
    return 1;
}

SC_FUNC void delete_heaplisttable(void)
{
    while (heaplist.next != NULL) {
        valuepair* cur = heaplist.next;
        heaplist.next = cur->next;
        free(cur);
    } /* while */
}

/* ----- debug information --------------------------------------- */

static stringlist dbgstrings = {NULL, NULL};

SC_FUNC stringlist* insert_dbgfile(const char* filename)
{

    if (sc_status == statWRITE && (sc_debug & sSYMBOLIC) != 0) {
        char string[_MAX_PATH + 40];
        assert(filename != NULL);
        assert(strlen(filename) + 40 < sizeof string);
        sprintf(string, "F:%08lx %s", (long)code_idx, filename);
        return insert_string(&dbgstrings, string);
    } /* if */
    return NULL;
}

SC_FUNC stringlist* insert_dbgline(int linenr)
{
    if (sc_status == statWRITE && (sc_debug & sSYMBOLIC) != 0) {
        char string[40];
        if (linenr > 0) {
            linenr--; /* line numbers are zero-based in the debug information */
        }
        sprintf(string, "L:%08lx %04x", (long)code_idx, linenr);
        return insert_string(&dbgstrings, string);
    } /* if */
    return NULL;
}

#ifdef WIN32
    #define LONGCAST long
#else
    #define LONGCAST cell
#endif

SC_FUNC stringlist* insert_dbgsymbol(symbol* sym)
{
    if (sc_status == statWRITE && (sc_debug & sSYMBOLIC) != 0) {
        char string[2 * sNAMEMAX + 128];
        char symname[2 * sNAMEMAX + 16];
#if !defined NDEBUG
        int count = 0;
#endif

        funcdisplayname(symname, sym->name);
        /* address tag:name codestart codeend ident vclass [tag:dim ...] */
#if PAWN_CELL_SIZE == 32
        if (sym->ident == iFUNCTN) {
            sprintf(string, "S:%08x %x:%s %08x %08x %x %x", sym->addr, sym->tag, symname, sym->addr, sym->codeaddr,
                sym->ident, sym->vclass);
        }
        else {
            sprintf(string, "S:%08x %x:%s %08x %08x %x %x", sym->addr, sym->tag, symname, sym->codeaddr, code_idx,
                sym->ident, sym->vclass);
        }
#elif PAWN_CELL_SIZE == 64
        if (sym->ident == iFUNCTN) {
            sprintf(string, "S:%08Lx %x:%s %08Lx %08Lx %x %x", (LONGCAST)sym->addr, sym->tag, symname, sym->addr,
                sym->codeaddr, sym->ident, sym->vclass);
        }
        else {
            sprintf(string, "S:%08Lx %x:%s %08Lx %08Lx %x %x", (LONGCAST)sym->addr, sym->tag, symname, sym->codeaddr,
                code_idx, sym->ident, sym->vclass);
        }
#endif
        if (sym->ident == iARRAY || sym->ident == iREFARRAY) {
#if !defined NDEBUG
            count = sym->dim.array.level;
#endif
            strcat(string, " [ ");
            for (const symbol* sub = sym; sub != NULL; sub = finddepend(sub)) {
#if !defined NDEBUG
                assert(sub->dim.array.level == count--);
#endif
                sprintf(string + strlen(string), "%x:%x ", sub->x.idxtag, sub->dim.array.length);
            } /* for */
            strcat(string, "]");
        } /* if */

        return insert_string(&dbgstrings, string);
    } /* if */
    return NULL;
}

SC_FUNC char* get_dbgstring(const int index)
{
    return get_string(&dbgstrings, index);
}

SC_FUNC void delete_dbgstringtable(void)
{
    delete_stringtable(&dbgstrings);
    assert(dbgstrings.next == NULL);
}
