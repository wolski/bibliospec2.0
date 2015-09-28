/*
  Copyright (c) 2011, University of Washington
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer. 
    * Redistributions in binary form must reproduce the above
    copyright notice, this list of conditions and the following
    disclaimer in the documentation and/or other materials provided
    with the distribution. 
    * Neither the name of the <ORGANIZATION> nor the names of its
    contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/
/*  
 *  BiblioSpec Version 1.0
 *  Copyright 2006 University of Washington. All rights reserved.
 *  Written by Barbara Frewen, Michael J. MacCoss, William Stafford Noble
 *  in the Department of Genome Sciences at the University of Washington.
 *  http://proteome.gs.washington.edu/
 *
 */
                                                                                  
//header file for DotProduct

#ifndef DOTPRODUCT_H
#define DOTPRODUCT_H

#include <vector>
#include <cmath>
#include "Match.h"

#ifdef _MSC_VER
#include <float.h>
#define isnan _isnan
#endif


namespace BiblioSpec {

class DotProduct
{
 private:
  void init();
  static double getAngle(const vector<PEAK_T>& exp, 
                         const vector<PEAK_T>& ref);

 public: 
  DotProduct();
  ~DotProduct();
  static void compare(Match& match); 
};

} // namespace

#endif //DOTPRODUCT_H

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * End:
 */
