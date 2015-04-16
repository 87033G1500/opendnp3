package com.automatak.render.dnp3.objects.generators

import com.automatak.render._
import com.automatak.render.dnp3.objects._

object FixedSizeGenerator {

  def header(x: FixedSize)(implicit i: Indentation): Iterator[String] = {

    def getFieldString(x: FixedSizeField): String = x.cppType + " " + x.name + ";"

    def typedefs(x: FixedSizeField): Iterator[String] = {
      if(x.name == "value") Iterator("typedef %s ValueType;".format(x.cppType))
      else Iterator.empty
    }

    def members: Iterator[String] =  x.fields.map(f => typedefs(f)).iterator.flatten ++ x.fields.map(f => getFieldString(f)).iterator

    def sizeSignature: Iterator[String] = Iterator("static uint32_t Size() { return %d; }".format(x.size))

    def readSignature: Iterator[String] = Iterator("static bool Read(openpal::ReadBufferView&, %s&);".format(x.name))

    def writeSignature: Iterator[String] = Iterator("static bool Write(const %s&, openpal::WriteBufferView&);".format(x.name))

    sizeSignature ++
    readSignature ++
    writeSignature ++
    space ++
    members

  }

  def implementation(x: FixedSize)(implicit i: Indentation): Iterator[String] = {

    def readSignature: Iterator[String] = Iterator("bool %s::Read(ReadBufferView& buffer, %s& output)".format(x.name, x.name))

    def writeSignature: Iterator[String] = Iterator("bool %s::Write(const %s& arg, openpal::WriteBufferView& buffer)".format(x.name, x.name))

    def fieldParams(name: String) : String = {
      x.fields.map(f => f.name).map(s => "%s.%s".format(name,s)).mkString(", ")
    }

    def readFunction: Iterator[String] = readSignature ++ bracket {
      Iterator("return Parse::Many(buffer, %s);".format(fieldParams("output")))
    }

    def writeFunction: Iterator[String] = writeSignature ++ bracket {
      Iterator("return Format::Many(buffer, %s);".format(fieldParams("arg")))
    }

    readFunction ++ space ++ writeFunction
  }
}
