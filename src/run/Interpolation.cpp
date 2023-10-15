/**
 * @file Interpolation.cpp Implementation of non-templated parts of
 * Interpolation class
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
 *
 * $URL: $
 * $Id: $
 */

#include "Interpolation.h"

namespace run {

int Interpolation::run(const boost::shared_ptr<context::InterpContext>& ctxtPtr,
                       util::MultiIndexMap* m) {
  switch (interpProfile_->getFieldOption().getId()) {
#ifdef FIELD_DYNAMIC32
    case runprofile::FieldOptionEnum::DYNAMIC32:
      return runDynamic32(ctxtPtr, m);
#endif
#ifdef FIELD_A32
    case runprofile::FieldOptionEnum::A32:
      return runA32(ctxtPtr, m);
#endif
#ifdef FIELD_B32
    case runprofile::FieldOptionEnum::B32:
      return runB32(ctxtPtr, m);
#endif
#ifdef FIELD_C32
    case runprofile::FieldOptionEnum::C32:
      return runC32(ctxtPtr, m);
#endif
#ifdef FIELD_AB32
    case runprofile::FieldOptionEnum::AB32:
      return runAB32(ctxtPtr, m);
#endif
#ifdef FIELD_BC32
    case runprofile::FieldOptionEnum::BC32:
      return runBC32(ctxtPtr, m);
#endif
#ifdef FIELD_AC32
    case runprofile::FieldOptionEnum::AC32:
      return runAC32(ctxtPtr, m);
#endif
#ifdef FIELD_CRT32
    case runprofile::FieldOptionEnum::CRT32:
      return runCRT32(ctxtPtr, m);
#endif
#ifdef FIELD_DYNAMIC64
    case runprofile::FieldOptionEnum::DYNAMIC64:
      return runDynamic64(ctxtPtr, m);
#endif
#ifdef FIELD_A64
    case runprofile::FieldOptionEnum::A64:
      return runA64(ctxtPtr, m);
#endif
#ifdef FIELD_B64
    case runprofile::FieldOptionEnum::B64:
      return runB64(ctxtPtr, m);
#endif
#ifdef FIELD_C64
    case runprofile::FieldOptionEnum::C64:
      return runC64(ctxtPtr, m);
#endif
#ifdef FIELD_AB64
    case runprofile::FieldOptionEnum::AB64:
      return runAB64(ctxtPtr, m);
#endif
#ifdef FIELD_BC64
    case runprofile::FieldOptionEnum::BC64:
      return runBC64(ctxtPtr, m);
#endif
#ifdef FIELD_AC64
    case runprofile::FieldOptionEnum::AC64:
      return runAC64(ctxtPtr, m);
#endif
#ifdef FIELD_CRT64
    case runprofile::FieldOptionEnum::CRT64:
      return runCRT64(ctxtPtr, m);
#endif
    case runprofile::FieldOptionEnum::INVALID:
    case runprofile::FieldOptionEnum::NA:
    case runprofile::FieldOptionEnum::UNKNOWN:
    default:
      break;
  }
  throw std::logic_error("Unknown/unimplemented Field Option");
}

}  // namespace run
