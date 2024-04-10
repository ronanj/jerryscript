/* Copyright JS Foundation and other contributors, http://js.foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  The lit-hashmap library is based on the public domain software
 *  available from https://github.com/sheredom/hashmap.h
 */

#include "lit-hashmap.h"

#include <stdlib.h>
#include <string.h>

#include "ecma-globals.h"

#include "lit-hashmap-swisstable.h"
#include "lit-hashmap.h"

/* Type definition: Typename, type:key, type:value */
CWISS_DECLARE_NODE_HASHMAP (LitHashMap, uint32_t, const ecma_string_t *);

struct hashmap_s
{
  LitHashMap map;
};

void
hashmap_init (hashmap_t *map)
{
  hashmap_t hashmap_on_the_heap = (struct hashmap_s *) malloc (sizeof (struct hashmap_s));
  LitHashMap hashmap_on_the_stack = LitHashMap_new (8);
  hashmap_on_the_heap->map = hashmap_on_the_stack;
  *map = hashmap_on_the_heap;
}

void
hashmap_put (hashmap_t hashmap, const ecma_string_t *const literal)
{
  LitHashMap_Entry e = { literal->u.hash, literal };
  LitHashMap_insert (&hashmap->map, &e);
}

const ecma_string_t *
hashmap_get (hashmap_t hashmap, const ecma_string_t *const literal)
{
  LitHashMap_Iter it = LitHashMap_find (&hashmap->map, &(literal->u.hash));
  LitHashMap_Entry *v = LitHashMap_Iter_get (&it);
  return v ? v->val : NULL;
}

void
hashmap_remove (hashmap_t hashmap, const ecma_string_t *const literal)
{
  LitHashMap_erase (&hashmap->map, &(literal->u.hash));
}

void
hashmap_destroy (hashmap_t *hashmap)
{
  if (*hashmap)
  {
    free (*hashmap);
    *hashmap = (hashmap_t) 0;
  }
}

// #if JERRY_LIT_HASHMAP
// #endif /* JERRY_LIT_HASHMAP */
