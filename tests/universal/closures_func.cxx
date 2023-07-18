//
// Copyright (c) 2022 ZettaScale Technology
//
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// http://www.eclipse.org/legal/epl-2.0, or the Apache License, Version 2.0
// which is available at https://www.apache.org/licenses/LICENSE-2.0.
//
// SPDX-License-Identifier: EPL-2.0 OR Apache-2.0
//
// Contributors:
//   ZettaScale Zenoh Team, <zenoh@zettascale.tech>
//
#include "zenoh.hxx"
using namespace zenoh;

#undef NDEBUG
#include <assert.h>

size_t gcnt = 1;

//
// Test for all variants of construnctng closures for handling `Reply`:
// - from functions
// - from object copies
// - from object references
// - from object rvalue references
// - from lambda expressions
//
// The Reply iteself is also can be handled

void on_reply(Reply) { gcnt *= 2; };
void on_reply_ref(Reply&) { gcnt *= 3; };
void on_reply_moveref(Reply&&) { gcnt *= 5; };

struct OnReply {
    void operator()(Reply) { gcnt *= 7; };
};

struct OnReplyRef {
    void operator()(Reply&) { gcnt *= 11; };
};

struct OnReplyMoveRef {
    void operator()(Reply&&) { gcnt *= 13; };
};

void test_closure_reply() {
    // Create closures from functions
    ClosureReply closure_reply_f(on_reply);
    ClosureReply closure_reply_f_ref(on_reply_ref);
    ClosureReply closure_reply_f_moveref(on_reply_moveref);

    // Create closures from reference to objects
    OnReply on_reply_o;
    ClosureReply closure_reply_o(on_reply_o);
    OnReplyRef on_reply_o_ref;
    ClosureReply closure_reply_o_ref(on_reply_o_ref);
    OnReplyMoveRef on_reply_o_moveref;
    ClosureReply closure_reply_o_moveref(on_reply_o_moveref);

    ClosureReply closure_reply_c([](Reply) { gcnt *= 17; });
    ClosureReply closure_reply_c_ref([](Reply&) { gcnt *= 19; });
    ClosureReply closure_reply_c_moveref([](Reply&&) { gcnt *= 23; });
}

void on_query_constref(const Query&) { gcnt *= 7; };

void on_sample_constref(const Sample&) { gcnt *= 11; };

void on_id_ref(const Id&) { gcnt *= 13; };

void on_hello(Hello) { gcnt *= 17; };
void on_hello_ref(Hello&) { gcnt *= 19; };
void on_hello_moveref(Hello&&) { gcnt *= 23; };

int main(int argc, char** argv) {
    ClosureReply closure_reply_f(on_reply);
    ClosureReply closure_reply_f_ref(on_reply_ref);
    ClosureReply closure_reply_f_moveref(on_reply_moveref);
    ClosureReply closure_reply([](Reply) { gcnt *= 31; });
    ClosureReply closure_reply_ref([](Reply&) { gcnt *= 37; });
    ClosureReply closure_reply_moveref([](Reply&&) { gcnt *= 41; });

    gcnt = 1;
    Reply reply(nullptr);
    closure_reply(reply);
    closure_reply_ref(reply);
    closure_reply_moveref(reply);
    closure_reply_f(reply);
    closure_reply_f_ref(reply);
    closure_reply_f_moveref(reply);
    assert(gcnt == 1 * 2 * 3 * 5 * 31 * 37 * 41);

    gcnt = 1;
    closure_reply(std::move(reply));
    closure_reply_ref(std::move(reply));
    closure_reply_moveref(std::move(reply));
    closure_reply_f(std::move(reply));
    closure_reply_f_ref(std::move(reply));
    closure_reply_f_moveref(std::move(reply));
    assert(gcnt == 1 * 2 * 3 * 5 * 31 * 37 * 41);

    // ClosureQuery closure_query([&cnt](const Query&) { cnt++; });
    // ClosureQuery closure_query_f(on_query_lv);

    // closure_query(nullptr);

    // closure_query_f(nullptr);

    // assert(cnt == 1);
    // assert(gcnt == 1);
    // cnt = gcnt = 0;

    // ClosureSample closure_sample([&cnt](const Sample&) { cnt++; });
    // ClosureSample closure_sample_f(on_sample_lv);

    // closure_sample(nullptr);
    // closure_sample_f(nullptr);

    // assert(cnt == 1);
    // assert(gcnt == 1);
    // cnt = gcnt = 0;

    // ClosureZid closure_zid([&cnt](const Id&) { cnt++; });
    // ClosureZid closure_zid_f(on_id_ptr);

    // closure_zid(nullptr);
    // closure_zid_f(nullptr);

    // assert(cnt == 1);
    // assert(gcnt == 1);
    // cnt = gcnt = 0;

    // ClosureHello closure_hello_lv([&cnt](Hello) { cnt++; });
    // ClosureHello closure_hello_rv([&cnt](Hello&) { cnt++; });
    // ClosureHello closure_hello_rlv([&cnt](Hello&&) { cnt++; });
    // ClosureHello closure_hello_flv(on_hello_lv);
    // ClosureHello closure_hello_frv(on_hello_rv);
    // ClosureHello closure_hello_frlv(on_hello_rlv);

    // Hello hello(nullptr);
    // closure_hello_lv(hello);
    // closure_hello_rv(hello);
    // closure_hello_rlv(hello);
    // closure_hello_flv(hello);
    // closure_hello_frv(hello);
    // closure_hello_frlv(hello);
    // closure_hello_lv(std::move(hello));
    // closure_hello_rv(std::move(hello));
    // closure_hello_rlv(std::move(hello));
    // closure_hello_flv(std::move(hello));
    // closure_hello_frv(std::move(hello));
    // closure_hello_frlv(std::move(hello));

    // assert(cnt == 6);
    // assert(gcnt == 6);
    // cnt = gcnt = 0;
}
