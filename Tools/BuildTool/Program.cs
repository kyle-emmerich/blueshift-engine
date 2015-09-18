using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BuildTool
{
    class Program
    {
        static void Main(string[] args)
        {
			var analyzer = new CodeAnalyzer();
            analyzer.AnalyzeHeader(
@"

REFLECT_CLASS(X, Y)
class TestClass 
	: public Inherited, public Inherited2 {
    public:
        
        REFLECT_PROPERTY()
        bool TestProperty;

}

"
                );

         

            System.Console.ReadKey();
        }
    }
}
