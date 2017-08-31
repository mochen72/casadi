// NOLINT(legal/copyright)
#define casadi_central_diff_mem casadi_central_diff_mem<T1>
template<typename T1>
int casadi_central_diff(casadi_central_diff_mem* m) {
  // Control loop
  const int wait_for_pos_pert=1, wait_for_neg_pert=2;
  switch (m->status) {
    case 0: break;
    case wait_for_pos_pert: goto back_from_pos_pert;
    case wait_for_neg_pert: goto back_from_neg_pert;
    default: return 0;
  }
  // Backup x and r
  casadi_copy(m->x, m->n_x, m->x0);
  casadi_copy(m->r, m->n_r, m->r0);
  // Loop over directions
  for (m->i=0; m->i<m->n_x; ++m->i) {
    // Perturb x in positive direction and call function
    m->x[m->i] += m->h/2;
    m->status = wait_for_pos_pert;
    return 1;
    back_from_pos_pert:
    casadi_copy(m->r, m->n_r, m->J + m->i*m->n_r);
    // Perturb x in negative direction and call function
    m->x[m->i] = m->x0[m->i] - m->h/2;
    m->status = wait_for_neg_pert;
    return 1;
    back_from_neg_pert:
    m->x[m->i] = m->x0[m->i];
    // Calculate finite difference approximation
    casadi_axpy(m->n_r, -1., m->r, m->J + m->i*m->n_r);
    casadi_scal(m->n_r, 1/m->h, m->J + m->i*m->n_r);
  }
  // Successful return, restore r
  casadi_copy(m->r0, m->n_r, m->r);
  m->status = 0;
  return 0;
}
#undef casadi_central_diff_mem