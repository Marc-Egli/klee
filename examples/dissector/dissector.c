#include "stdio.h"
#include "customIncludes.h"
#include "stdlib.h"
int simple_dissect(tvbuff_t *input, proto_tree *tree){
        int offset = 0; 
    klee_make_symbolic(&offset,sizeof(offset),"offset");
    offset = 0;
    if(input->real_data[0] >  127) {
    
        if(input->real_data[0] == 255){
        printf("Entering Path 1.1\n");
        proto_tree_add_item(tree, 0, input, offset, 1, 0);
        offset += 1;
        }else{
        printf("Entering Path 1.2\n");
        proto_tree_add_item(tree, 0, input, offset, 1, 0);
        offset += 1;
        }
    } else {
        printf("Entering Path 2\n");  
        int size = input->real_data[1];
        // seq = proto_tree_add_item()
        proto_tree_add_item(tree, 1, input, offset, 1, 0);
        offset += 1;
        printf("Reading second part of the packet\n");
        proto_tree_add_item(tree, 2, input, offset, size, 0);
    }
    
    /* Actually dumps all constraints */
    klee_stack_trace(); 
    return offset;
}
int main(void) {
    //const unsigned char *data = malloc(sizeof(char) * 2);
    char data[2];   
    klee_make_symbolic(&data,sizeof(data),"data");
    tvbuff_t buf;
    proto_tree tree;
    buf = (tvbuff_t) {NULL, true, 0, NULL, data, 0, 0, 0};
    tree = (proto_tree) {NULL, NULL, NULL, NULL};   
    //dissect_foo(tvbuff_t *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_)
    
    int r = simple_dissect(&buf, &tree);
    return 0;
}
//Basic dissector from the wireshark tutorial
/*
int dissect_foo(tvbuff *tvb, packet_info *pinfo, proto_tree *tree _U_, void *data _U_) {
    gint offset = 0;
    // Sets wireshark protocol column to "FOO"
    col_set_str(pinfo->cinfo, COL_PROTOCOL, "FOO");
    // Clear the info column
    col_clear(pinfo->cinfo,COL_INFO);
    // Foo does not encapsulate other protocol data -> we consume all the date  from 0 to -1 (End)
    // ENC_NA -> No specifc encoding  such as Little or Big Endian
    // 
    proto_item *ti = proto_tree_add_item(tree, proto_foo, tvb, 0, -1, ENC_NA);
    // Adds a child node to the protocol tree. This expansion is controled by the ett_foo var.
    proto_tree *foo_tree = proto_item_add_subtree(ti, ett_foo);
    // 1 byte, starting at 0, format is Big Endian.
    proto_tree_add_item(foo_tree, hf_foo_pdu_type, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset += 1;
    proto_tree_add_item(foo_tree, hf_foo_flags, tvb, offset, 1, ENC_BIG_ENDIAN);
    offset += 1;
    proto_tree_add_item(foo_tree, hf_foo_sequenceno, tvb, offset, 2, ENC_BIG_ENDIAN);
    offset += 2;
    proto_tree_add_item(foo_tree, hf_foo_initialip, tvb, offset, 4, ENC_BIG_ENDIAN);
    offset += 4;
    return tvb_captured_length(tvb);    
}
*/
