/**
 * @file FieldBenchmarks.cpp Implementation of non-templated parts of
 * FieldBenchmarks class
 *
 * Copyright (C) 2010  Arthur D. Cherba
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "FieldBenchmarks.h"

namespace run {

int FieldBenchmarks::run(util::MultiIndexMap* mmap) {
#ifdef FIELD_DYNAMIC32
  runDynamic32(mmap);
#endif
#ifdef FIELD_A32
  runA32(mmap);
#endif
#ifdef FIELD_B32
  runB32(mmap);
#endif
#ifdef FIELD_C32
  runC32(mmap);
#endif
#ifdef FIELD_AB32
  runAB32(mmap);
#endif
#ifdef FIELD_BC32
  runBC32(mmap);
#endif
#ifdef FIELD_AC32
  runBC32(mmap);
#endif
#ifdef FIELD_CRT32
  runCRT32(mmap);
#endif
#ifdef FIELD_DYNAMIC64
  runDynamic64(mmap);
#endif
#ifdef FIELD_A64
  runA64(mmap);
#endif
#ifdef FIELD_B64
  runB64(mmap);
#endif
#ifdef FIELD_C64
  runC64(mmap);
#endif
#ifdef FIELD_AB64
  runAB64(mmap);
#endif
#ifdef FIELD_BC64
  runBC64(mmap);
#endif
#ifdef FIELD_AC64
  runBC64(mmap);
#endif
#ifdef FIELD_CRT64
  runCRT64(mmap);
#endif
  return 0;
}

}  // namespace run
