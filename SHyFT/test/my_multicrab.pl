#!/usr/bin/perl

# The input record separator is defined by Perl global
# variable $/.  It can be anything, including multiple
# characters.  Normally it is "\n", newline.  Here, we
# say there is no record separator, so the whole file
# is read as one long record, newlines included.
undef $/;

# get the inputs
$crab=$ARGV[0];
$cmssw=$ARGV[1];
$dataset=$ARGV[2];
$outdir=$ARGV[3];
$number=$ARGV[4];
$njobs=$ARGV[5];
$scheduler=$ARGV[6];
$server=$ARGV[7];
$curr_dir=$ARGV[8];
$ui_dir=$ARGV[9];

print "usage: ./my_multicrab.pl crab.cfg cmssw.py /my/dataset /my/outdir number njobs scheduler={condor,glite} server={0,1} curr_dir ui_dir={default,etc}\n\n";

print "Using $crab as a template\n";
print "Changing:\n";
print "cmssw python file = $cmssw \n";
print "dataset           = $dataset \n";
print "outdir            = $outdir \n";
print "number of events  = $number \n";
print "number of jobs    = $njobs \n";
print "scheduler         = $scheduler \n";
print "crab server       = $server \n";
print "curr dir          = $curr_dir\n";
print "ui dir            = $ui_dir\n";

# open the file
if (! open(INPUT,"<$crab") ) {
  print STDERR "Can't open file $crab\n";
  exit(0);
}

# slurp it in as one long record
$data = <INPUT>;
close INPUT;

# do the substitutions
$data =~ s/DUMMY_CMSSW/$cmssw/g;
$data =~ s/DUMMY_DATASET/$dataset/g;
$data =~ s/DUMMY_OUTDIR/$outdir/g;
$data =~ s/DUMMY_NUMBER/$number/g;
$data =~ s/DUMMY_NJOBS/$njobs/g;
$data =~ s/DUMMY_USE_SERVER/$server/g;
$data =~ s/DUMMY_SCHEDULER/$scheduler/g;
$ttemp = $curr_dir . "/" . $ui_dir;
$data =~ s/DUMMY_UI_DIR/$ttemp/g;

if ( $ui_dir eq 'default' ) {
  $data =~ s/ui_working_dir/#ui_working_dir/g;
}


# rename the output file
$outname = $crab;
$outname =~ s/dummy/$ui_dir/g;

print "Printing to $outname\n";

# open the output file
if (! open(OUTPUT, ">$outname") ) {
  die "Can't open output file $outname\n";
}

#print "Data is :\n";
#print $data;
#print "\n";

# print out the output
print OUTPUT $data;
close OUTPUT;
print STDERR "$outname was written\n";

print "Executing : crab -create -cfg $outname \n";
system "crab -create -cfg $outname \n";
print "Executing : crab -submit -c $ui_dir \n";
system "crab -submit -c $ui_dir \n";


exit(0);
