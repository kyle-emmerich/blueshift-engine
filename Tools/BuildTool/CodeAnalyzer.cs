using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.IO;

namespace BuildTool
{
    public class CodeAnalyzer
    {
		public static string ClassDefinition = "REFLECT_CLASS";
		public static string PropertyDefinition = "REFLECT_PROPERTY";


        public enum CodeType : byte
        {
            SignedInt32, UnsignedInt32,
            SignedInt64, UnsignedInt64,
            Bool,
            Float, Double,
            SignedByte, UnsignedByte,
            String,
            ClassPointer, ClassReference,
            Array, Map, Set
        }
		public enum CodeAccessLevel : byte
		{
			Private,
			Protected,
			Public
		}
        public struct CodeProperty
        {
            public string Name;

            public CodeType Type;
            public bool ConstantType;

            public bool IsContainer;
            public CodeType ContainerType;           
        }
        public struct CodeFunctionArgument
        {
            public string Name;

            public CodeType Type;
            public bool ConstantType;

            public bool Optional;
        }
        public struct CodeFunction
        {
            public string Name;
            public List<CodeFunctionArgument> Arguments;
            public CodeType ReturnType;
            public bool ConstantReturnType;

            public bool IsConstant;
        }
		public struct CodeParentClass
		{
			public CodeAccessLevel Access;
			public string Name;
		}
        public class CodeClass
        {
            public string Name;
            public List<CodeProperty> Properties;
            public List<CodeFunction> Functions;

            public List<CodeParentClass> InheritsFrom = new List<CodeParentClass>();
        }

		public List<CodeClass> Classes;
		private CodeClass current;

        public void AnalyzeHeader(string HeaderContent)
        {
			Regex find_comments = new Regex(@"/\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+/", RegexOptions.ExplicitCapture);
			Regex find_class_definition = new Regex(@"class\s*([\w_:\.]+)\s*(?::\s*([^{]+)|class\s*([\w_:\.]+))?");

			int index = 0;
			while (index != -1)
			{
				//Search for the next ClassDefinition string
				index = HeaderContent.IndexOf(ClassDefinition, index);

				if (index != -1)
				{
					//We found one, let's check it out.
					//We first need to find just this line (no multiline definitions!)
					int line_end = HeaderContent.IndexOf('\n', index);
					//Now, we only need the arguments, so skip over the length of the ClassDefinition + 1 for the (
					int arg_start = index + ClassDefinition.Length + 1;
					string arguments = HeaderContent.Substring(arg_start, line_end - arg_start - 2);

					//Split the arguments into a list!
					var arg_list = this.ParseList(arguments);
					
					foreach (string x in arg_list)
					{
						
					//	Console.WriteLine(x.Trim());
					}

					index = line_end + 1;

					//Now let's find the associated class definition.
					var match = find_class_definition.Match(HeaderContent, index);
					Console.WriteLine(match.Success);
					if (match.Success)
					{
						current = new CodeClass();
						current.Name = match.Groups[1].Value;

						//Do we have an inheritance list?
						if (match.Groups.Count > 2)
						{
							var inherited_list = this.ParseList(match.Groups[2].Value);

							foreach (string parent_class in inherited_list)
							{
								CodeParentClass parent = new CodeParentClass();
								parent.Access = CodeAccessLevel.Private;

								//Does our string start with public?
								string p = parent_class.Trim();
								if (p.Substring(0, "public".Length) == "public")
								{
									parent.Access = CodeAccessLevel.Public;
									p = p.Substring("public".Length + 1);
								}
								if (p.Substring(0, "protected".Length) == "protected")
								{
									parent.Access = CodeAccessLevel.Private;
									p = p.Substring("protected".Length + 1);
								}
								if (p.Substring(0, "private".Length) == "private")
								{
									parent.Access = CodeAccessLevel.Private;
									p = p.Substring("private".Length + 1);
								}

								parent.Name = p.Trim();

								current.InheritsFrom.Add(parent);
							}
						}
					}
				}
			}
        }

		private Regex find_comments = new Regex(@"/\*([^*]|[\r\n]|(\*+([^*/]|[\r\n])))*\*+/", RegexOptions.ExplicitCapture);
		public List<string> ParseList(string items)
		{
			items = items.Trim();
			items = find_comments.Replace(items, string.Empty);

			List<string> output = new List<string>();

			int depth = 0;
			StringBuilder builder = new StringBuilder();
			foreach (char c in items)
			{
				switch (c)
				{
					case ',':
						if (depth == 0 && builder.Length > 0)
						{
							//ding
							output.Add(builder.ToString().Trim());
							builder.Clear();
						}
						break;
					case '<':
						depth++;
						break;
					case '>':
						depth--;
						break;
					default:
						builder.Append(c);
						break;
				}

				
			}
			if (builder.Length > 0)
			{
				output.Add(builder.ToString().Trim());
			}

			return output;
		}
    }
}
