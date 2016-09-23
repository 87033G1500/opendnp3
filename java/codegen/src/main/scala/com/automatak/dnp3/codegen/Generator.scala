package com.automatak.dnp3.codegen

import java.nio.file.FileSystems

import com.automatak.dnp3._
import com.automatak.dnp3.enums.TimeSyncMode

object Generator {

  implicit val indent = CppIndentation

  val javaGenPath = FileSystems.getDefault.getPath("./cpp/adapters/JNIStrings.h");

  // all the classes to generate C++ info on
  def classes : List[ClassConfig] = List(

    ClassConfig(classOf[MasterStackConfig], Features.Fields),
    ClassConfig(classOf[MasterConfig], Features.FQCN, Features.Fields),
    ClassConfig(classOf[LinkLayerConfig], Features.FQCN, Features.Fields),
    ClassConfig(classOf[LogEntry], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[LogHandler], Features.Methods),
    ClassConfig(classOf[TimeSyncMode], Features.FQCN),
    ClassConfig(classOf[java.time.Duration], Features.FQCN),
    ClassConfig(classOf[ClassField], Features.FQCN, Features.Fields)

  )

  def main(args: Array[String]): Unit = {

    writeTo(javaGenPath)(ClassInfoGenerator.lines(classes))
  }



}
