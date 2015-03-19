package com.automatak.render.dnp3.objects.generators

import java.nio.file.Path
import com.automatak.render.dnp3.objects._
import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.LicenseHeader

object GroupVariationFileGenerator {

  def apply(path: Path) = {

    implicit val indent = CppIndentation()

    def headerPath(group: ObjectGroup): Path = path.resolve(group.name+".h")
    def implPath(group: ObjectGroup): Path = path.resolve(group.name+".cpp")

    def definitions(r: ModelRenderer[GroupVariation])(group: ObjectGroup): Iterator[String] = spaced(group.objects.iterator.map(o => r.render(o)))

    def optionalIncludes(group: ObjectGroup) : Set[String] = {

      def getEnums(gv: GroupVariation):  List[String] = {

        def extract(typ: FixedSizeFieldType): Option[String] = typ match {
          case EnumField(model) => Some(quoted("opendnp3/gen/"+model.name+".h"))
          case _ => None
        }

        gv match {
          case fs : FixedSize => fs.fields.flatMap(f => extract(f.typ))
          case _ => Nil
        }
      }

      def getConversionHeaders(gv: GroupVariation):  List[String] = {

        gv match {
          case fs : FixedSize => fs.conversion match {
            case Some(con) => con.includeHeaders
            case None => Nil
          }
          case _ => Nil
        }
      }

      def include(file: String): String = "#include " + file

      group.objects.flatMap(o => getEnums(o) ++ getConversionHeaders(o)).map(include).toSet
    }

    def includeHeader(group: ObjectGroup): Iterator[String] = Iterator("#include " + quoted(group.name+".h"))

    def optionalCppIncludes(group: ObjectGroup) : Set[String] = {

      def getConversionHeaders(gv: GroupVariation):  List[String] = gv match {
          case fs : FixedSize => fs.conversion match {
            case Some(con) => con.implHeaders
            case None => Nil
          }
          case _ => Nil
      }

      def include(file: String): String = "#include " + file

      group.objects.flatMap(o => getConversionHeaders(o)).map(include).toSet
    }

    def headerFile(group: ObjectGroup): Iterator[String] = {
      commented(LicenseHeader()) ++ space ++
      includeGuards(group.name.toUpperCase) {
          Iterator("#include <openpal/container/ReadBufferView.h>") ++
          Iterator("#include <openpal/container/WriteBufferView.h>") ++
          Iterator("""#include "opendnp3/Types.h"""") ++
          Iterator("""#include "opendnp3/app/GroupVariationID.h"""") ++
        optionalIncludes(group) ++ space ++
        namespace("opendnp3") {
          definitions(GroupVariationHeaderRenderer)(group)
        }
      }
    }

    def implFile(group: ObjectGroup): Iterator[String] = {
      val defs = definitions(GroupVariationImplRenderer)(group)
      if(defs.isEmpty) Iterator.empty
      else
      {
        commented(LicenseHeader()) ++ space ++
          includeHeader(group) ++ space ++
          optionalCppIncludes(group) ++
          Iterator("#include <openpal/serialization/Format.h>") ++
          Iterator("#include <openpal/serialization/Parse.h>") ++ space ++
          Iterator("using namespace openpal;") ++ space ++
          namespace("opendnp3") {
            defs
          }
      }
    }

    ObjectGroup.all.foreach { g =>
        writeTo(headerPath(g))(headerFile(g))
        (writeTo(implPath(g))(implFile(g)))
    }
  }

}
