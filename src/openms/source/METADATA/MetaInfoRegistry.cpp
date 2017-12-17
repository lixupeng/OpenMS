// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2016.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Hendrik Weisser $
// $Authors: Marc Sturm, Hendrik Weisser $
// -------------------------------------------------------------------------

#include <sstream>

#include <OpenMS/METADATA/MetaInfoRegistry.h>

namespace OpenMS
{

    MetaInfoRegistry::MetaInfoRegistry() :
            next_index_(1024), name_to_index_(), index_to_name_(), index_to_description_(), index_to_unit_()
    {
      insert(name_to_index_, String("isotopic_range"), UInt(1));
      insert(index_to_name_, UInt(1), String("isotopic_range"));
      insert(index_to_description_, UInt(1), String("consecutive numbering of the peaks in an isotope pattern. 0 is the monoisotopic peak"));
      insert(index_to_unit_, UInt(1), String(""));

      insert(name_to_index_, String("cluster_id"), UInt(2));
      insert(index_to_name_, UInt(2), String("cluster_id"));
      insert(index_to_description_, UInt(2), String("consecutive numbering of isotope clusters in a spectrum"));
      insert(index_to_unit_, UInt(2),String(""));

      insert(name_to_index_, String("label"), UInt(3));
      insert(index_to_name_, UInt(3), String("label"));
      insert(index_to_description_, UInt(3), String("label e.g. shown in visialization"));
      insert(index_to_unit_, UInt(3), String(""));

      insert(name_to_index_, String("icon"), UInt(4));
      insert(index_to_name_, UInt(4), String("icon"));
      insert(index_to_description_, UInt(4), String("icon shown in visialization"));
      insert(index_to_unit_, UInt(4), String(""));

      insert(name_to_index_, String("color"), UInt(5));
      insert(index_to_name_, UInt(5), String("color"));
      insert(index_to_description_, UInt(5), String("color used for visialization e.g. #FF00FF for purple"));
      insert(index_to_unit_, UInt(5), String(""));

      insert(name_to_index_, String("RT"), UInt(6));
      insert(index_to_name_, UInt(6), String("RT"));
      insert(index_to_description_, UInt(6), String("the retention time of an identification"));
      insert(index_to_unit_, UInt(6), String(""));

      insert(name_to_index_, String("MZ"), UInt(7));
      insert(index_to_name_, UInt(7), String("MZ"));
      insert(index_to_description_, UInt(7), String("the MZ of an identification"));
      insert(index_to_unit_, UInt(7), String(""));

      insert(name_to_index_, String("predicted_RT"), UInt(8));
      insert(index_to_name_, UInt(8), String("predicted_RT"));
      insert(index_to_description_, UInt(8), String("the predicted retention time of a peptide hit"));
      insert(index_to_unit_, UInt(8), String(""));

      insert(name_to_index_, String("predicted_RT_p_value"), UInt(9));
      insert(index_to_name_, UInt(9), String("predicted_RT_p_value"));
      insert(index_to_description_, UInt(9), String("the predicted RT p-value of a peptide hit"));
      insert(index_to_unit_, UInt(9), String(""));

      insert(name_to_index_, String("spectrum_reference"), UInt(10));
      insert(index_to_name_, UInt(10), String("spectrum_reference"));
      insert(index_to_description_, UInt(10), String("Refenference to a spectrum or feature number"));
      insert(index_to_unit_, UInt(10), String(""));

      insert(name_to_index_, String("ID"), UInt(11));
      insert(index_to_name_, UInt(11), String("ID"));
      insert(index_to_description_, UInt(11), String("Some type of identifier"));
      insert(index_to_unit_, UInt(11), String(""));

      insert(name_to_index_, String("low_quality"), UInt(12));
      insert(index_to_name_, UInt(12), String("low_quality"));
      insert(index_to_description_, UInt(12), String("Flag which indicatest that some entity has a low quality (e.g. a feature pair)"));
      insert(index_to_unit_, UInt(12), String(""));

      insert(name_to_index_, String("charge"), UInt(13));
      insert(index_to_name_, UInt(13), String("charge"));
      insert(index_to_description_, UInt(13), String("Charge of a feature or peak"));
      insert(index_to_unit_, UInt(13), String(""));
    }

    MetaInfoRegistry::MetaInfoRegistry(const MetaInfoRegistry& rhs)
    {
      *this = rhs;
    }

    MetaInfoRegistry::~MetaInfoRegistry()
    {
    }

    MetaInfoRegistry& MetaInfoRegistry::operator=(const MetaInfoRegistry& rhs)
    {
      if (this == &rhs) return *this;
      index_to_description_ = rhs.index_to_description_;
      index_to_unit_ = rhs.index_to_unit_;
      index_to_name_ = rhs.index_to_name_;
      name_to_index_ = rhs.name_to_index_;
      next_index_.store(rhs.next_index_);
      return *this;
    }

    UInt MetaInfoRegistry::registerName(const String& name, const String& description, const String& unit)
    {
      UInt rv;
      if (!find(name_to_index_, name, rv))
      {
        rv = next_index_++;
        insert(name_to_index_, name, rv);
        insert(index_to_name_, rv, name);
        insert(index_to_description_, rv, description);
        insert(index_to_unit_, rv, unit);
      }
      return rv;
    }

    void MetaInfoRegistry::setDescription(UInt index, const String& description)
    {
      if (!insert_if_exist(index_to_description_, index, description))
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered index!", String(index));
      }
    }

    void MetaInfoRegistry::setDescription(const String& name, const String& description)
    {
      UInt id;
      if (!find(name_to_index_, name, id))
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered name!", name);
      }
      insert(index_to_description_, id, description);
    }

    void MetaInfoRegistry::setUnit(UInt index, const String& unit)
    {
      if (!insert_if_exist(index_to_unit_, index, unit))
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered index!", String(index));
      }
    }

    void MetaInfoRegistry::setUnit(const String& name, const String& unit)
    {
      UInt id;
      if (!find(name_to_index_, name, id))
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered name!", name);
      }
      insert(index_to_unit_, id, unit);
    }

    UInt MetaInfoRegistry::getIndex(const String& name) const
    {
      UInt id = UInt(-1);
      find(name_to_index_, name, id);
      return id;
    }

    String MetaInfoRegistry::getDescription(UInt index) const
    {
      String result;
      if (!find(index_to_description_, index, result))
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered index!", String(index));
      }
      return result;
    }

    String MetaInfoRegistry::getDescription(const String& name) const
    {
      String rv;
      UInt id = getIndex(name);
      if (id == UInt(-1)) // not found
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered Name!", name);
      }
      else
      {
        find(index_to_description_, id, rv);
      }
      return rv;
    }

    String MetaInfoRegistry::getUnit(UInt index) const
    {
      String result;
      if (!find(index_to_unit_, index, result))
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered index!", String(index));
      }
      return result;
    }

    String MetaInfoRegistry::getUnit(const String& name) const
    {
      String rv;
      UInt id = getIndex(name);
      if (id == UInt(-1)) // not found
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered Name!", name);
      }
      else
      {
        find(index_to_unit_, id, rv);
      }
      return rv;
    }

    String MetaInfoRegistry::getName(UInt index) const
    {
      String rv;
      if (!find(index_to_name_, index, rv))
      {
        throw Exception::InvalidValue(__FILE__, __LINE__, OPENMS_PRETTY_FUNCTION, "Unregistered index!", String(index));
      }
      return rv;
    }
} //namespace
