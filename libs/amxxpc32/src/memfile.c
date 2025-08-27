// vim: set ts=4 sw=4 tw=99 noet:
//
// AMX Mod X, based on AMX Mod by Aleksander Naszko ("OLO").
// Copyright (C) The AMX Mod X Development Team.
//
// This software is licensed under the GNU General Public License, version 3 or higher.
// Additional exceptions apply. For full license details, see LICENSE.txt or visit:
//     https://alliedmods.net/amxmodx-license

#include "memfile.h"
#include "osdefs.h"
#include <string.h>

memfile_t* memfile_creat(const char* name, const size_t init)
{
    memfile_t* pmf = malloc(sizeof(memfile_t));
    if (!pmf) {
        return NULL;
    }

    pmf->size = init;
    pmf->base = (char*)malloc(init);
    if (!pmf->base) {
        free(pmf);
        return NULL;
    }

    pmf->usedoffs = 0;
    pmf->offs = 0;
    pmf->_static = 0;

#if defined _MSC_VER
    pmf->name = _strdup(name);
#else
    pmf->name = strdup(name);
#endif

    if (!pmf->name) {
        free(pmf->base);
        free(pmf);
        return NULL;
    }

    return pmf;
}

void memfile_destroy(memfile_t* mf)
{
    if (!mf->_static) {
        free(mf->name);
        free(mf->base);
        free(mf);
    }
}

void memfile_seek(memfile_t* mf, const long seek)
{
    mf->offs = seek;
}

long memfile_tell(const memfile_t* mf)
{
    return mf->offs;
}

size_t memfile_read(memfile_t* mf, void* buffer, size_t maxsize)
{
    if (!maxsize || mf->offs >= mf->usedoffs) {
        return 0;
    }

    if (mf->usedoffs - mf->offs < (long)maxsize) {
        maxsize = mf->usedoffs - mf->offs;
        if (!maxsize) {
            return 0;
        }
    }

    memcpy(buffer, mf->base + mf->offs, maxsize);

    mf->offs += maxsize;

    return maxsize;
}

int memfile_write(memfile_t* mf, const void* buffer, const size_t size)
{
    if (mf->offs + size > mf->size) {
        const size_t newsize = (mf->size + size) * 2;
        char* newbase;
        if (mf->_static) {
            newbase = (char*)malloc(newsize);
            if (!newbase) {
                return 0;
            }
            memcpy(newbase, mf->base, mf->size);
        }
        else {
            newbase = (char*)realloc(mf->base, newsize);
            if (!newbase) {
                return 0;
            }
        }
        mf->base = newbase;
        mf->_static = 0;
        mf->size = newsize;
    }
    memcpy(mf->base + mf->offs, buffer, size);
    mf->offs += size;

    if (mf->offs > mf->usedoffs) {
        mf->usedoffs = mf->offs;
    }

    return 1;
}
