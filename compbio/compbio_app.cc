#include <core/types.hh>
#include <core/init.hh>
#include <core/scoring/constraints/util.hh>
#include <utility/exit.hh>
#include <utility/excn/EXCN_Base.hh>
#include <utility/string_util.hh>
#include <basic/Tracer.hh>
// option key includes
#include <basic/options/option.hh>
#include <basic/options/after_opts.hh>
#include <basic/options/keys/run.OptionKeys.gen.hh>
#include <basic/options/keys/out.OptionKeys.gen.hh>
#include <utility/excn/Exceptions.hh>

//# RBO_abinitio code
#include <sys/stat.h>
#include <apps/pilot/compbio/compbio_abinitio.hh>

#include <string>
#include <basic/options/keys/constraints.OptionKeys.gen.hh>

static basic::Tracer tr( "apps.pilot.compbio" );

int
main( int argc, char * argv [] )
{
    using namespace basic::options;
    using namespace basic::options::OptionKeys;
    compbio_abinitio::register_options();
    core::init( argc, argv );

    std::cout << "cst_weight: " << option[constraints::cst_weight] << std::endl;

    if ( option[out::nstruct] && option[out::nstruct] > 0 )
    {
        tr.Info << "nstruct option: " << option[out::nstruct] << std::endl;
        int num_decoys = option[out::nstruct];
        for ( int i = 0; i < num_decoys+1; i++ )
        {
            compbio_abinitio ab( i );
            ab.run();
        }
    }
    else
    {
        compbio_abinitio ab( 0 );
        ab.run();
    }

//     struct stat st;
//     if ( stat( "./models", &st ) == 0 )
//     {
//       system( "rm -vr models" );
//     }

    system( "mkdir -v models" );
    std::string mv = "mv -v ";
    system( (mv + option[out::output_tag] + "model_*.pdb models").c_str() );
    system( (mv + option[out::output_tag] + "score.fsc models").c_str() );

    return 0;
}
