import sys,ROOT,os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE(s)\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

import ROOT
#ROOT.gSystem.Load("libOpFlashAna_OpT0FinderApp")
from larlite import larlite as fmwk
from ROOT import flashana

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)):
    if not sys.argv[x].endswith('.root'):
        continue
    print sys.argv[x]
    my_proc.add_input_file(sys.argv[x])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("ana.root")

# Attach an analysis unit ... here we use a base class which does nothing.
# Replace with your analysis unit if you wish.
my_unit = fmwk.FlashMatch2D()
my_proc.add_process(my_unit)
my_unit.SetConfigFile("dl_one_to_many.fcl")

print
print  "Finished configuring ana_processor. Start event loop!"
print
fout1=open('data.txt','w')
fout1.write('xmin,xmax,time,x,t,q,oppe,hypope\n')
ctr=0
fout2=open('scoremap.txt','w')
line=''
for x in xrange(48):
    line+='score%02d,' % x
line=line.rstrip(',')

fout2.write('%s\n' % line)

while 1:
    
    my_proc.process_event(ctr)

    match_v = my_unit.MatchResult()

    mgr = my_unit.Manager()

    qcluster_v = mgr.QClusterArray()

    flash_v = mgr.FlashArray()

    #print match_v.size(),'match found for',qcluster_v.size(),'tpc objects and',flash_v.size(),'pmt objects'
    score_array=[0]*48
    skip=False
    for index in xrange(match_v.size()):

        #print 'Match id',index
        match = match_v[index]
        #print 'TPC:',match.tpc_id,'PMT:',match.flash_id,'Score:',match.score,'TPC point:',match.tpc_point.x,match.tpc_point.y,match.tpc_point.z
        #print match.tpc_id,len(score_array)
        score_array[match.tpc_id] = match.score
        if skip: continue
        if not skip: skip=True
        xmin = 1e4
        xmax = 0
        qc=None
        try:
            qc = qcluster_v[match.tpc_id]
        except IndexError:
            print
            print match.tpc_id,'/',qcluster_v.size()
            print
            raise IndexError
        for pt_idx in xrange(qc.size()):
            pt = qc[pt_idx]
            if pt.x > xmax: xmax = pt.x
            if pt.x < xmin: xmin = pt.x

        #print 'xmin:',xmin,'xmax:',xmax
        flash = flash_v[match_v[index].flash_id]
        pesum = 0
        for pe in flash.pe_v: pesum += pe
        hyposum=0
        for pe in match.hypothesis: hyposum += pe

        subvolume_wires = my_unit.SubVolumeWires()
        subvolume_ticks = my_unit.SubVolumeTicks()
        nsubh = 3456 / subvolume_wires
        
        if nsubh * subvolume_wires % 3456: nsubh += 1

        print 'HBox:',  match.tpc_id % nsubh, 'VBox:', match.tpc_id / nsubh
        volz = ((match.tpc_id % nsubh) + 0.5) * subvolume_wires * 0.3
        volx = ((match.tpc_id / nsubh) + 0.5) * subvolume_ticks
        
        fout1.write('%g,%g,%g,%g,%g,%g,%g,%g\n' % (xmin,xmax,qc.time,match.tpc_point.x,flash.time,match.score,pesum,hyposum))    

    line=''
    for s in score_array: line+='%g,' % s
    line = line.rstrip(',')
    line+='\n'
    fout2.write(line)
    if ctr > 10: break
    proc_status = my_proc.get_process_status()
    if proc_status == my_proc.kFINISHED:
        break
    ctr+=1
    sys.stdout.write('%d\r' % ctr)
    sys.stdout.flush()

    if 'debug' in sys.argv:
        cmd=sys.stdin.readline().rstrip('\n')
        if cmd.lower() in ['n','no']: break
# done!
print
print "Finished running ana_processor event loop!"
print
fout1.close()
fout2.close()
sys.exit(0)
