package com.automatak.dnp3.codegen

import java.lang.reflect.{Constructor, Method}

object JNIMethod {

  val classOfBool = classOf[java.lang.Boolean]
  val classOfShort = classOf[java.lang.Short]
  val classOfInt = classOf[java.lang.Integer]
  val classOfLong = classOf[java.lang.Long]
  val classOfFloat = classOf[java.lang.Float]
  val classOfDouble = classOf[java.lang.Double]
  val classOfString = classOf[java.lang.String]

  def getType(clazz: Class[_]): String = clazz match {
    //case `classOfVoid` => "void"
    case `classOfShort` => "jshort"
    case `classOfInt` => "jint"
    case `classOfLong` => "jlong"
    case `classOfFloat` => "jfloat"
    case `classOfDouble` => "jdouble"
    case `classOfString` => "jstring"
    case _ => {
      if(clazz.isPrimitive) {
        clazz.getTypeName match {
          case "void" => "void"
          case _ => throw new Exception("undefined primitive type: %s".format(clazz.getTypeName))
        }
      }
      "jobject"
    }
  }

  def getFieldType(clazz: Class[_]): String = clazz match {

    case `classOfInt` => "I"
    case `classOfLong` => "J"
    case `classOfBool` => "Z"
    case `classOfDouble` => "D"
    case _ => {
      if(clazz.isPrimitive) {
        throw new Exception("undefined primitive type: %s".format(clazz.getTypeName))
      }
      clazz.fqcn
    }
  }

  def getSignature(method: Method, className: Option[String] = None) : String = {

    def returnType = getType(method.getReturnType)

    def arguments = method.getParameters.map(p => "%s %s".format(p.getName, getType(p.getType.getClass))).mkString(", ")

    "%s %s%s(JNIEnv* env, %s)".format(returnType, className.map(n => "%s::".format(n)).getOrElse(""), method.getName, arguments)
  }

  def getConstructorSignature(method: Constructor[_], className: Option[String] = None) : String = {


    def arguments = method.getParameters.map(p => "%s %s".format(p.getName, getType(p.getType.getClass))).mkString(", ")

    "jobject %sinit%d(JNIEnv* env, %s)".format(className.map(n => "%s::".format(n)).getOrElse(""), method.getParameterCount, arguments)

  }
}
