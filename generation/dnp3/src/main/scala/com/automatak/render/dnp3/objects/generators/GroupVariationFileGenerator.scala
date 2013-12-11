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
          case EnumField(model) => Some("<opendnp3/gen/"+model.name+".h>")
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
            case Some(con) => con.headers
            case None => Nil
          }
          case _ => Nil
        }
      }

      def include(file: String): String = "#include " + file

      group.objects.flatMap(o => getEnums(o) ++ getConversionHeaders(o)).map(include).toSet
    }

    def includeHeader(group: ObjectGroup): Iterator[String] = Iterator("#include " + quoted(group.name+".h"))

    def headerFile(group: ObjectGroup): Iterator[String] = {
      commented(LicenseHeader()) ++ space ++
      Iterator("#include <openpal/BufferWrapper.h>") ++
      optionalIncludes(group) ++ space ++
      namespace("opendnp3") {
        definitions(GroupVariationHeaderRenderer)(group)
      }
    }

    def implFile(group: ObjectGroup): Iterator[String] = {
      commented(LicenseHeader()) ++ space ++
        includeHeader(group) ++ space ++
        Iterator("#include <openpal/Serialization.h>") ++ space ++
        Iterator("using namespace openpal;") ++ space ++
        namespace("opendnp3") {
          definitions(GroupVariationImplRenderer)(group)
        }
    }

    ObjectGroup.all.foreach(g => writeTo(headerPath(g))(headerFile(g)))
    ObjectGroup.all.foreach(g => writeTo(implPath(g))(implFile(g)))
  }

}
